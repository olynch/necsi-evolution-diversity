cmake_minimum_required (VERSION 2.8)
project (necsi-evolution-diversity)

set (necsi-evolution-diversity_VERSION_MAJOR 0)
set (necsi-evolution-diversity_VERSION_MINOR 0)
set (necsi-evolution-diversity_VERSION_PATCH 1)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O2")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CTOML_INCLUDE "ctoml/include")
set(CTOML "ctoml")
include_directories(${CTOML_INCLUDE})

add_executable(Test test_env.cpp environment.cpp ${CTOML}/toml.cc ${CTOML}/tomlvalue.cc)

set(CMAKE_BUILD_TYPE Debug)
