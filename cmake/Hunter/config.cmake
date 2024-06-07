
hunter_config(
    Eigen
    VERSION 3.4.0
)
hunter_config(
    basalt-headers
    VERSION 0.1.0
    URL "https://github.com/luxonis/basalt-headers/archive/refs/heads/hunter.zip"
    SHA1 "740cdd6ea5bcb303e1b2334530f85a7dbf4a644d"
)
hunter_config(
    Sophus
    VERSION 1.22.10
    URL "https://github.com/luxonis/Sophus/archive/refs/heads/hunterized.zip"
    SHA1 "ff17d3dce1bcadabbc9fb83845a29974377d7f95" 
    CMAKE_ARGS
        BUILD_SOPHUS_TESTS=OFF
        BUILD_SOPHUS_EXAMPLES=OFF
)

hunter_config(
    cereal
    VERSION 1.3.0
    URL "https://github.com/USCiLab/cereal/archive/64f50dbd5cecdaba785217e2b0aeea3a4f1cdfab.zip"
    SHA1 "6587337e36045f1b5f68c902f5e04a76d8999e02"
)



hunter_config(
    magic_enum
    VERSION 0.7.3
    URL "https://github.com/Neargye/magic_enum/archive/3d1f6a5a2a3fbcba077e00ad0ccc2dd9fefc2ca7.zip"
    SHA1 "c9a27f6ff8311f0c6b2adb959d0598f079fcc9f3"
)

hunter_config(
    opengv
    VERSION 1.0.0
    URL "https://github.com/luxonis/opengv/archive/refs/heads/master.zip"
    SHA1 "5898069592f3e7484d9e83d918dad9287876d103"
)