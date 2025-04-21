cmake_minimum_required(VERSION 3.25)
project(StochasticSim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE
        STRING
        "Configuration type (one of Debug, Release)"
        FORCE
  )
endif()

if(MSVC)
    add_compile_options(
        /W4            # Warning level 4
        #/WX            # Treat warnings as errors
        /permissive-   # Strict standards conformance
        /MP            # Multi-processor compilation
    )
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /sdl /analyze /guard:cf")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Oi /Ot /GL")
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror        # Treat warnings as errors
        -Wshadow       # Warn about shadowed variables
        -Wnon-virtual-dtor
        -pedantic-errors
    )
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# --- Qt Configuration ---
# Find all required Qt components centrally
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)
# Set AUTOMOC, AUTOUIC, AUTORCC globally for targets needing them
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# --- End Qt Configuration ---

# Headers
add_library(CommonIncludes INTERFACE)
target_include_directories(CommonIncludes
    INTERFACE Include
)

## GTESTS
#option(BUILD_TESTING "Build the tests for StochasticSim" ON) # Add option
#
#if(BUILD_TESTING)
#  message(STATUS "Building Tests")
#  include(FetchContent)
#  FetchContent_Declare(
#    googletest
#    GIT_REPOSITORY https://github.com/google/googletest.git
#    GIT_TAG    v1.16.0
#  )
#  set(INSTALL_GTEST OFF CACHE BOOL "Disable installing GTest")
#  FetchContent_MakeAvailable(googletest)
#  enable_testing()
#  add_subdirectory(Tests)
#else()
#    message(STATUS "Skipping Tests")
#endif()

# Adjust subdirectory order for dependency clarity
add_subdirectory(src/Model)
add_subdirectory(src/Presenter) # Presenter before View
add_subdirectory(src/View)

add_executable(StochasticSim
    src/main.cpp
    src/AppInitializer.cpp
)
target_include_directories(StochasticSim PRIVATE
    src
    Include
    ${Qt6Core_INCLUDE_DIRS}      # Explicitly add Qt Core includes
    ${Qt6Widgets_INCLUDE_DIRS}   # Explicitly add Qt Widgets includes
    ${Qt6Gui_INCLUDE_DIRS}       # Explicitly add Qt Gui includes (for QPalette, QColor)
)
# Link the executable against necessary libraries (including Qt if main.cpp uses it)
target_link_libraries(StochasticSim PRIVATE
    Model
    View
    Presenter
    Qt6::Core      # Link main executable if needed
    Qt6::Widgets   # Link main executable if needed
    Qt6::Gui       # Link Gui module (for QPalette, QColor)
)

# windeployqt for DLLs
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
    add_custom_command(TARGET StochasticSim POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:StochasticSim>"
        COMMENT "Deploying Qt dependencies..."
    )
endif()
