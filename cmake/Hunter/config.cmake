
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
    URL "https://github.com/luxonis/basalt-headers/archive/e3ee456469f21a356a0a59088779b32721918f11.zip"
    SHA1 "a3fb1771957493df7ae460693e8fad7eaac54b16"
)
hunter_config(
    Sophus
    VERSION 1.22.10
    URL "https://github.com/luxonis/Sophus/archive/54e9b230edc4df47f819cef0d15b1fcc165342df.zip"
    SHA1 "53493ab699bf1ef5d3d8ab2892f8eaa80cf1dfc3" 
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
    CMAKE_CXX_VISIBILITY_PRESET=hidden
    CMAKE_C_VISIBILITY_PRESET=hidden
    )


hunter_config(
    magic_enum
    VERSION 0.7.3
    URL "https://github.com/Neargye/magic_enum/archive/3d1f6a5a2a3fbcba077e00ad0ccc2dd9fefc2ca7.zip"
    SHA1 "c9a27f6ff8311f0c6b2adb959d0598f079fcc9f3"
)

