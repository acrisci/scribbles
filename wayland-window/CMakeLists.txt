cmake_minimum_required(VERSION 3.8)
project(untitled)

list(INSERT CMAKE_MODULE_PATH 0
	${CMAKE_CURRENT_SOURCE_DIR}/CMake
)

find_package(Wayland REQUIRED)
find_package(WaylandProtocols REQUIRED)
find_package(EGL REQUIRED)
find_package(GLESv2 REQUIRED)
find_package(DRM REQUIRED)
find_package(GBM REQUIRED)
find_package(LibInput REQUIRED)
find_package(XKBCommon REQUIRED)
find_package(Udev REQUIRED)
find_package(Libcap)
find_package(Systemd)

include_directories(
        shared
		${WAYLAND_INCLUDE_DIR}
		${WAYLAND_PROTOCOLS_DIR}
)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c shared/os-compatibility.c)
add_executable(shm-test ${SOURCE_FILES})

target_link_libraries(
		shm-test
		${WAYLAND_LIBRARIES}
)


add_executable(egl-test main-egl.c)

target_link_libraries(
		egl-test
		${WAYLAND_LIBRARIES}
        ${EGL_LIBRARIES}
        ${GLESv2_LIBRARIES}
)
