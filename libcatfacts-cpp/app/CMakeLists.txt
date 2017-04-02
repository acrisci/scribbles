project(catfactscli)

add_executable(catfactscli main.cpp)

set(catfactscli_SOURCES main.cpp)

include_directories(
    ${PROJECT_SOURCE_DIR}/../libcatfacts/include
)
#target_link_libraries(libcatfacts)
#target_link_libraries(catfactscli ${CMAKE_SOURCE_DIR}/build/libcatfacts/libcatfacts.a)
target_link_libraries(catfactscli libcatfacts)
