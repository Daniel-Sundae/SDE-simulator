cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(StochasticSim VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Additional settings
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_DEBUG_POSTFIX "d")

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

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

set(SOURCES
    src/main.cpp
    src/SDE.cpp
    src/MainWindow.cpp
    src/ChartWidget.cpp
    src/ButtonWidget.cpp
    src/SDEChart.cpp
)

file(GLOB_RECURSE CORE_HEADERS "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE UTILS_HEADERS "${PROJECT_SOURCE_DIR}/include/Utils/*.hpp")
set(HEADERS
    ${CORE_HEADERS}
    ${UTILS_HEADERS}
)

add_executable(StochasticSim
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(StochasticSim PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Utils
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6Charts_INCLUDE_DIRS}
)

# Link Qt libraries
target_link_libraries(StochasticSim PRIVATE 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Charts
)

# Set bundle properties for macOS or properties for other platforms (e.g., Windows)
if(APPLE)
    set_target_properties(StochasticSim PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourorg.stochasticsim"
        OUTPUT_NAME_DEBUG "StochasticSim_Debug"
        OUTPUT_NAME_RELEASE "StochasticSim_Release"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
    )
else()
    # Windows-specific properties
    set_target_properties(StochasticSim PROPERTIES
        WIN32_EXECUTABLE TRUE  # Indicates a Windows GUI application
        OUTPUT_NAME_DEBUG "StochasticSim_Debug"
        OUTPUT_NAME_RELEASE "StochasticSim_Release"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
    )
endif()
