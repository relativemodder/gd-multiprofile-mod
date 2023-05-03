cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gd-multiprofile-mod VERSION 1.0.0)

file(GLOB SOURCES
	src/*.cpp
	src/*.hpp
	layers/*.hpp
	layers/*.cpp
	objects/*.hpp
	objects/*.cpp
	utils/*.hpp
	utils/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} "src/utils/Game.hpp")

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} $ENV{GEODE_SDK}/build)

setup_geode_mod(${PROJECT_NAME})
