cmake_minimum_required(VERSION 3.15)
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

# Headers
add_library(CommonIncludes INTERFACE)
target_include_directories(CommonIncludes
    INTERFACE Include
)


# GTESTS
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG    v1.16.0
)
set(INSTALL_GTEST OFF CACHE BOOL "Disable installing GTest")
FetchContent_MakeAvailable(googletest)
enable_testing()

add_subdirectory(Tests)
add_subdirectory(src/Model)
add_subdirectory(src/View)
add_subdirectory(src/Presenter)
add_executable(StochasticSim
    src/main.cpp
    src/AppInitializer.cpp
)
target_include_directories(StochasticSim PRIVATE
    src
    Include
)
target_link_libraries(StochasticSim PRIVATE
    Model
    View
    Presenter
)

# windeployqt for DLLs
if(WIN32)
    add_custom_command(TARGET StochasticSim POST_BUILD
        COMMAND "${CMAKE_PREFIX_PATH}/bin/windeployqt.exe" "$<TARGET_FILE:StochasticSim>"
        COMMENT "Deploying Qt dependencies..."
    )
endif()