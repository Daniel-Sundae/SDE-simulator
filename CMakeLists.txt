cmake_minimum_required(VERSION 3.22)
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

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts OpenGL OpenGLWidgets)
find_package(GTest REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

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
    Qt6::Charts
    Qt6::OpenGL 
    Qt6::OpenGLWidgets
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(WINDEPLOYQT_SCRIPT "windeployqt.debug.bat")
    message("Using Windeploy Debug")
else()
    set(WINDEPLOYQT_SCRIPT "windeployqt6.exe")
    message("Using Windeploy Release")
endif()
add_custom_command(TARGET StochasticSim POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/tools/Qt6/bin/${WINDEPLOYQT_SCRIPT} $<TARGET_FILE:StochasticSim>
    COMMENT "Deploying Qt libraries"
)