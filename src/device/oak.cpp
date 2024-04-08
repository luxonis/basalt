/**
BSD 3-Clause License

This file is part of the Basalt project.
https://gitlab.com/VladyslavUsenko/basalt.git

Copyright (c) 2019, Vladyslav Usenko, Michael Loipführer and Nikolaus Demmel.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "basalt/device/oak.h"
#include "basalt/utils/assert.h"

std::string get_date();

namespace basalt {

OAKDevice::OAKDevice() : manual_exposure(false), skip_frames(0), webp_quality(100), frame_counter(0) {
  // initialize oak-d pipeline
  pipeline = std::make_shared<dai::Pipeline>();

  // Define sources and outputs for stereo pair
  auto monoLeft = pipeline->create<dai::node::MonoCamera>();
  auto monoRight = pipeline->create<dai::node::MonoCamera>();
  auto imu = pipeline->create<dai::node::IMU>();
  auto sync = pipeline->create<dai::node::Sync>();

  auto stereo = pipeline->create<dai::node::StereoDepth>();

  auto xoutSync = pipeline->create<dai::node::XLinkOut>();
  auto xoutIMU = pipeline->create<dai::node::XLinkOut>();
  auto xoutDepth = pipeline->create<dai::node::XLinkOut>();

  xoutSync->setStreamName("sync");
  xoutIMU->setStreamName("imu");
  xoutDepth->setStreamName("depth");

  monoLeft->setBoardSocket(dai::CameraBoardSocket::LEFT);
  monoLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_400_P);
  monoLeft->setFps(60);

  monoRight->setBoardSocket(dai::CameraBoardSocket::RIGHT);
  monoRight->setResolution(dai::MonoCameraProperties::SensorResolution::THE_400_P);
  monoRight->setFps(60);

  // Create a node that will produce the depth map
  stereo->setConfidenceThreshold(200);
  stereo->setRectifyEdgeFillColor(0);  // black, to better see the cutout
  stereo->setMedianFilter(dai::MedianFilter::KERNEL_7x7);

  imu->enableIMUSensor({dai::IMUSensor::ACCELEROMETER_RAW, dai::IMUSensor::GYROSCOPE_RAW}, 200);
  imu->setBatchReportThreshold(1);
  imu->setMaxBatchReports(10);

  // Linking
  monoLeft->out.link(stereo->left);
  monoRight->out.link(stereo->right);

  stereo->rectifiedLeft.link(sync->inputs["left"]);
  stereo->rectifiedRight.link(sync->inputs["right"]);
  stereo->depth.link(xoutDepth->input);
  sync->out.link(xoutSync->input);

  // Connect IMU
  imu->out.link(xoutIMU->input);
}

void OAKDevice::imgCallback(std::shared_ptr<dai::ADatatype> images) {
  auto group = std::dynamic_pointer_cast<dai::MessageGroup>(images);
  int i = 0;
  OpticalFlowInput::Ptr data(new OpticalFlowInput(2));
  for (auto& msg : *group) {
    std::shared_ptr<dai::ImgFrame> imgFrame = std::dynamic_pointer_cast<dai::ImgFrame>(msg.second);
    auto t = imgFrame->getTimestamp();
    int64_t t_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
    auto exposure = imgFrame->getExposureTime();

    int exposure_ms = std::chrono::duration_cast<std::chrono::milliseconds>(exposure).count();
    data->img_data[i].img.reset(new basalt::ManagedImage<uint16_t>(imgFrame->getWidth(), imgFrame->getHeight()));
    data->t_ns = t_ns;
    data->img_data[i].exposure = exposure_ms;
    size_t full_size = imgFrame->getWidth() * imgFrame->getHeight();
    const uint8_t* data_in = imgFrame->getData().data();
    uint16_t* data_out = data->img_data[i].img->ptr;
    for (size_t j = 0; j < full_size; j++) {
      int val = data_in[j];
      val = val << 8;
      data_out[j] = val;
    }
    i++;
  }
  last_img_data = data;
  if (image_data_queue) {
    image_data_queue->push(data);
  }
}

void OAKDevice::imuCallback(std::shared_ptr<dai::ADatatype> imu_data) {
  auto imuPackets = std::dynamic_pointer_cast<dai::IMUData>(imu_data);

  for (auto& imuPacket : imuPackets->packets) {
    ImuData<double>::Ptr data;
    data.reset(new basalt::ImuData<double>);
    auto t = imuPacket.acceleroMeter.getTimestamp();
    int64_t t_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

    data->t_ns = t_ns;
    data->accel = Eigen::Vector3d(imuPacket.acceleroMeter.x, imuPacket.acceleroMeter.y, imuPacket.acceleroMeter.z);
    data->gyro = Eigen::Vector3d(imuPacket.gyroscope.x, imuPacket.gyroscope.y, imuPacket.gyroscope.z);
    if (imu_data_queue) imu_data_queue->push(data);
  }
}

void OAKDevice::disableLaserEmitters() {
  //   std::vector<rs2::sensor> sensors = pipe.get_active_profile().get_device().query_sensors();
  //   for (auto&& sensor : sensors) {
  //     if (sensor.is<rs2::depth_stereo_sensor>()) {
  //       sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 0);
  //     }
  //   }
}

void OAKDevice::start() {
  // start the device
  device = std::make_shared<dai::Device>(*pipeline);
  device->startPipeline();

  // get the output queues
  syncQueue = device->getOutputQueue("sync", 8, false);
  imuQueue = device->getOutputQueue("imu", 8, false);
  depthQueue = device->getOutputQueue("depth", 8, false);
  // add callbacks

  syncQueue->addCallback(std::bind(&OAKDevice::imgCallback, this, std::placeholders::_1));
  imuQueue->addCallback(std::bind(&OAKDevice::imuCallback, this, std::placeholders::_1));
}

void OAKDevice::stop() {
  if (image_data_queue) image_data_queue->push(nullptr);
  if (imu_data_queue) imu_data_queue->push(nullptr);
}

bool OAKDevice::setExposure(double exposure) {
  // if (!manual_exposure) return false;

  // sensor.set_option(rs2_option::RS2_OPTION_EXPOSURE, exposure * 1000);
  return true;
}

void OAKDevice::setSkipFrames(int skip) { skip_frames = skip; }

void OAKDevice::setWebpQuality(int quality) { webp_quality = quality; }

std::shared_ptr<basalt::Calibration<double>> OAKDevice::exportCalibration() {
  using Scalar = double;

  if (calib.get()) return calib;

  calib.reset(new basalt::Calibration<Scalar>);
  calib->imu_update_rate = IMU_RATE;

  // get camera ex-/intrinsics
  auto calibHandler = device->readCalibration2();


  // update after extrinsics are available
  // auto imuLeftExtrinsics = calibHandler.getCameraToImuExtrinsics(dai::CameraBoardSocket::LEFT);
  // auto imuRightExtrinsics = calibHandler.getCameraToImuExtrinsics(dai::CameraBoardSocket::RIGHT);
  // std vector of std vectors to Eigen::Matrix

  // Eigen::Matrix<Scalar, 4, 4> imuLeftExtrinsicsMatrix;
  // Eigen::Matrix<Scalar, 4, 4> imuRightExtrinsicsMatrix;

  // for (int i = 0; i < 4; i++) {
  //   for (int j = 0; j < 4; j++) {
  //     imuLeftExtrinsicsMatrix(i, j) = imuLeftExtrinsics[i][j];
  //     imuRightExtrinsicsMatrix(i, j) = imuRightExtrinsics[i][j];
  //   }
  // }
  // Eigen::Matrix3d rot = Eigen::Map<Eigen::Matrix3f>(ex.rotation);

  // For OAK D-PRO
  double roll = -3.1415;
  double pitch = 0.0;
  double yaw = -1.5708;
  Eigen::AngleAxisd roll_angle(roll, Eigen::Vector3d::UnitX());
  Eigen::AngleAxisd pitch_angle(pitch, Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd yaw_angle(yaw, Eigen::Vector3d::UnitZ());

  Eigen::Quaterniond qL = roll_angle * pitch_angle * yaw_angle;

  Eigen::Quaterniond qR = roll_angle * pitch_angle * yaw_angle;
  // OAK D PRO
  Eigen::Vector3d transL(0.0, -0.06935, -0.00565); // y x z for some reason
  Eigen::Vector3d transR(0.0,0.0, -0.00565);

  // OAK D PRO W
  // Eigen::Vector3d transL(0.0, -0.075448, -0.0048);  // y x z for some reason
  // Eigen::Vector3d transR(0.0,0.0, -0.0048);


  basalt::Calibration<Scalar>::SE3 T_i_c_left(qL,transL);
  basalt::Calibration<Scalar>::SE3 T_i_c_right(qR,transR);

  calib->T_i_c.push_back(T_i_c_left);
  calib->T_i_c.push_back(T_i_c_right);

  // get resolution

  Eigen::Vector2i resolution;
  resolution << 640, 400;
  calib->resolution.push_back(resolution);

  auto leftIntrinsics = calibHandler.getCameraIntrinsics(dai::CameraBoardSocket::LEFT, 640, 400);
  auto rightIntrinsics = calibHandler.getCameraIntrinsics(dai::CameraBoardSocket::RIGHT, 640, 400);

  basalt::GenericCamera<Scalar> cameraL;
  basalt::PinholeCamera<Scalar>::VecN paramsL;
  // fx, fy, cx, cy
  double fxL = leftIntrinsics[0][0];
  double fyL = leftIntrinsics[1][1];
  double cxL = leftIntrinsics[0][2];
  double cyL = leftIntrinsics[1][2];
  paramsL << fxL, fyL, cxL, cyL;

  basalt::PinholeCamera<Scalar> pinholeL(paramsL);
  cameraL.variant = pinholeL;

  calib->intrinsics.push_back(cameraL);

  basalt::GenericCamera<Scalar> cameraR;
  basalt::PinholeCamera<Scalar>::VecN paramsR;
  // fx, fy, cx, cy
  double fxR = rightIntrinsics[0][0];
  double fyR = rightIntrinsics[1][1];
  double cxR = rightIntrinsics[0][2];
  double cyR = rightIntrinsics[1][2];

  paramsR << fxR, fyR, cxR, cyR;

  basalt::PinholeCamera<Scalar> pinholeR(paramsR);
  cameraR.variant = pinholeR;

  calib->intrinsics.push_back(cameraR);

  return calib;
}

}  // namespace basalt
