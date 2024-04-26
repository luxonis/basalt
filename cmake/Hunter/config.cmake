
hunter_config(
    Eigen
    VERSION 3.4.0
    CMAKE_ARGS
        CMAKE_BUILD_TYPE=RelWithDebInfo
        EIGEN_INITIALIZE_MATRICES_BY_NAN=ON
)

hunter_config(
    Sophus
    URL "https://github.com/strasdat/Sophus/archive/936265f5815dee546a3583ceba58c234c2ecb271.zip"
    SHA1 "7f54e06fb1680910c465e12ea85fc414ce7a9f9e" 
    CMAKE_ARGS
    CMAKE_BUILD_TYPE=RelWithDebInfo
)

hunter_config(
    cereal
    URL "https://github.com/USCiLab/cereal/archive/64f50dbd5cecdaba785217e2b0aeea3a4f1cdfab.zip"
    SHA1 "6587337e36045f1b5f68c902f5e04a76d8999e02"
)

hunter_config(
    basalt-headers
    URL "https://github.com/Serafadam/basalt-headers-mirror/archive/refs/heads/hunter.zip"
    SHA1 "0998433c242cf1f615441032aa954c92e42daa5f"
)

hunter_config(
    magic_enum
    URL "https://github.com/Neargye/magic_enum/archive/3d1f6a5a2a3fbcba077e00ad0ccc2dd9fefc2ca7.zip"
    SHA1 "c9a27f6ff8311f0c6b2adb959d0598f079fcc9f3"
)