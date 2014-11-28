cmake_minimum_required(VERSION 2.8)
project(necsi-evolution-diversity)

set (necsi-evolution-diversity_VERSION_MAJOR 0)
set (necsi-evolution-diversity_VERSION_MINOR 0)
set (necsi-evolution-diversity_VERSION_PATCH 1)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_executable(test_prob_dist test_prob_dist.c prob_dist.c)
target_link_libraries(test_prob_dist ${PYTHON_LIBRARIES})

set(CMAKE_BUILD_TYPE Debug)
