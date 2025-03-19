cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(StochasticSim VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add strict compiler options based on compiler type
if(MSVC)
    add_compile_options(
        /W4     # Warning level 4
        /WX     # Treat warnings as errors
        /permissive- # Strict standards conformance
        /analyze # Enable code analysis
        /sdl    # Enable additional security checks
        #/Zi     # Generate debug info
        /guard:cf # Control Flow Guard
        /MP     # Multi-processor compilation
    )
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror     # Treat warnings as errors
        -Wconversion # Warn about implicit conversions
        -Wshadow    # Warn about shadowed variables
        -Wnon-virtual-dtor # Warn about non-virtual destructors
        -pedantic-errors  # Strict ISO C++ compliance
    )
endif()

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
if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
endif()

# Set automoc, autorcc, and autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt components
find_package(Qt6 COMPONENTS Core Widgets Charts REQUIRED)


file(GLOB_RECURSE SRC_IMPL
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE SRC_HEADERS
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
)
set(SOURCES ${SRC_IMPL})
set(HEADERS ${SRC_HEADERS})
add_executable(StochasticSim
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(StochasticSim PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/Model
    ${PROJECT_SOURCE_DIR}/src/Model/Calculations
    ${PROJECT_SOURCE_DIR}/src/Model/Processes
    ${PROJECT_SOURCE_DIR}/src/View
    ${PROJECT_SOURCE_DIR}/src/View/Outputs
    ${PROJECT_SOURCE_DIR}/src/View/Inputs
    ${PROJECT_SOURCE_DIR}/src/Presenter
)

# Link Qt libraries
target_link_libraries(StochasticSim PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
)

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
    set_target_properties(StochasticSim PROPERTIES
        WIN32_EXECUTABLE TRUE  # Indicates a Windows GUI application
        OUTPUT_NAME_DEBUG "StochasticSim_Debug"
        OUTPUT_NAME_RELEASE "StochasticSim_Release"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
    )
endif()
