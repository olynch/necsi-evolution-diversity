cmake_minimum_required(VERSION 2.8)
project(necsi-evolution-diversity)

set (necsi-evolution-diversity_VERSION_MAJOR 0)
set (necsi-evolution-diversity_VERSION_MINOR 0)
set (necsi-evolution-diversity_VERSION_PATCH 1)
set (GSL_DIR .)

find_package(PythonLibs REQUIRED)
find_package(GSL REQUIRED)

set (INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS} ${GSL_INCLUDE_DIR})
include_directories(${INCLUDE_DIRS})

add_executable(test_prob_dist test_prob_dist.c prob_dist.c)
target_link_libraries(test_prob_dist ${PYTHON_LIBRARIES})

add_executable(test_square test_square.c square.c common.c prob_dist.c)
target_link_libraries(test_square ${GSL_LIBRARIES} ${PYTHON_LIBRARIES})

add_executable(test_grid test_grid.c grid.c square.c prob_dist.c common.c)
target_link_libraries(test_grid ${GSL_LIBRARIES} ${PYTHON_LIBRARIES})

set(CMAKE_BUILD_TYPE Debug)
