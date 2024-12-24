cmake_minimum_required(VERSION 3.15)
project(StochasticSim VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define build types if not already defined
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Set compiler flags based on the build type
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")

# Add compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set automoc, autorcc, and autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages
if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/mingw_64/lib/cmake")
else()
    set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@6")
endif()

# Find required Qt components
find_package(Qt6 COMPONENTS Core Widgets Charts REQUIRED)

# Add executable
add_executable(StochasticSim
    src/main.cpp
    src/SDE.cpp
    # Add other source files here
)

# Link Qt libraries
target_link_libraries(StochasticSim PRIVATE 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Charts
)

# Set macOS bundle properties
if(APPLE)
    set_target_properties(StochasticSim PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourorg.stochasticsim"
        MACOSX_BUNDLE_BUNDLE_NAME "StochasticSim"
    )
endif()

# Set output directories based on build type
set_target_properties(StochasticSim PROPERTIES
  OUTPUT_NAME_DEBUG StochasticSim_Debug
  OUTPUT_NAME_RELEASE StochasticSim_Release
)
