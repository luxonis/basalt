

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO luxonis/basalt-headers
    REF e3ee456469f21a356a0a59088779b32721918f11
    SHA512 08d35d3a24fa84fc58d42d5788c5da4198ab58924f455179f81bc14ee6ed5ee54f0dafd0c5323633a2ce207412a4534d9484ae9ee7695e2bc1288ea402ab0c96
    HEAD_REF hunter
)
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

