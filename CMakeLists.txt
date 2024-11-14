# COMMENT ON HOW TO BUILD RELEASE AND DEBUG
# HAVE TO BE IN BUILD DIRECTORY 
set(DOCUMENTATION_TEXT
  [[
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build . --config Release

    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cmake --build . --config Debug
  ]]
)


cmake_minimum_required(VERSION 3.15)
project(StochasticSim VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Define build types
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Add executable
file(GLOB SOURCES "src/*.cpp")
add_executable(StochasticSim ${SOURCES})

# Set output directories based on build type
set_target_properties(StochasticSim PROPERTIES
  OUTPUT_NAME_DEBUG StochasticSim_Debug
  OUTPUT_NAME_RELEASE StochasticSim_Release
)