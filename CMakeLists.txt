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
        /WX-            # Do not treat warnings as errors
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
        #-Werror        # Treat warnings as errors
        -Wshadow
        -Wnon-virtual-dtor
        -pedantic-errors
    )
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(CommonIncludes INTERFACE)
target_include_directories(CommonIncludes
    INTERFACE Include
)

# Google tests
option(BUILD_TESTING "Build the tests" OFF)
if(BUILD_TESTING)
    find_package(GTest CONFIG QUIET)
    if(GTest_FOUND)
        message(STATUS "Using system-installed GTest")
    else()
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        )
        FetchContent_MakeAvailable(googletest)
    endif()
    enable_testing()
    add_subdirectory(Tests)
endif()

add_subdirectory(src/Model)
add_subdirectory(src/Presenter)
add_subdirectory(src/View)

add_executable(StochasticSim
    src/main.cpp
    src/AppInitializer.cpp
)
target_include_directories(StochasticSim PRIVATE
    src
    Include
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6Gui_INCLUDE_DIRS}
)
target_link_libraries(StochasticSim PRIVATE
    Model
    View
    Presenter
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

# windeployqt for DLLs
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
    add_custom_command(TARGET StochasticSim POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:StochasticSim>"
        COMMENT "Deploying Qt dependencies..."
    )
endif()
