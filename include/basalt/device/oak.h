
#pragma once

#include <math.h>
#include <atomic>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <thread>

#include "depthai/depthai.hpp"

#include <pangolin/image/typed_image.h>
#include <pangolin/pangolin.h>

#include <tbb/concurrent_queue.h>

#include <basalt/imu/imu_types.h>
#include <basalt/optical_flow/optical_flow.h>
#include <basalt/calibration/calibration.hpp>

namespace basalt {



class OAKDevice {
 public:
  using Ptr = std::shared_ptr<OAKDevice>;

  static constexpr int IMU_RATE = 200;
  static constexpr int NUM_CAMS = 2;

  OAKDevice();

  void start();
  void stop();

  bool setExposure(double exposure);  // in milliseconds
  void setSkipFrames(int skip);
  void setWebpQuality(int quality);

  void imgCallback(std::shared_ptr<dai::ADatatype> images);
  void imuCallback(std::shared_ptr<dai::ADatatype> imu_data);

  std::shared_ptr<basalt::Calibration<double>> exportCalibration();

  OpticalFlowInput::Ptr last_img_data;
  tbb::concurrent_bounded_queue<OpticalFlowInput::Ptr>* image_data_queue = nullptr;
  tbb::concurrent_bounded_queue<ImuData<double>::Ptr>* imu_data_queue = nullptr;

 private:
  void disableLaserEmitters();

  bool manual_exposure;
  int skip_frames;
  int webp_quality;

  int frame_counter = 0;

  std::shared_ptr<basalt::Calibration<double>> calib;

  std::shared_ptr<dai::Pipeline> pipeline;
  std::shared_ptr<dai::Device> device;
  std::shared_ptr<dai::DataOutputQueue> syncQueue;
  std::shared_ptr<dai::DataOutputQueue> imuQueue;
  std::shared_ptr<dai::DataOutputQueue> depthQueue;

};

}  // namespace basalt
