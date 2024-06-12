
hunter_config(
    Eigen
    VERSION 3.4.0
    CMAKE_ARGS
        EIGEN_DONT_PARALLELIZE=ON
        EIGEN_INITIALIZE_MATRICES_BY_NAN=ON
)

hunter_config(
    oneTBB
    VERSION 2021.12.0
    URL "https://github.com/oneapi-src/oneTBB/archive/refs/tags/v2021.12.0.zip"
    SHA1 "f6b0eb4e45af600684282341115a3c2fb9834978"
    CMAKE_ARGS
        TBB_PREVIEW_GLOBAL_CONTROL=ON
        TBB_TEST=OFF
        CMAKE_CXX_VISIBILITY_PRESET=hidden
        CMAKE_C_VISIBILITY_PRESET=hidden
)

hunter_config(
    basalt-headers
    VERSION 0.1.0
    URL "https://github.com/luxonis/basalt-headers/archive/refs/heads/hunter.zip"
    SHA1 "958c2c322fd9ff902cdd45139255e88219ac7079"
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
    fmt
    VERSION 8.1.1
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