project(libcatfacts)
set(libcatfacts_SOURCES src/libcatfacts.cpp)

find_package(PkgConfig)
pkg_check_modules(CURL libcurl REQUIRED)

add_library(libcatfacts ${libcatfacts_SOURCES})

include_directories(
    ${CURL_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(libcatfacts
    ${CURL_LIBRARIES}
)
