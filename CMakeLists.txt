cmake_minimum_required(VERSION 3.22)
project(StochasticSim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
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

if(NOT MSVC)
  message(FATAL_ERROR "This project requires MSVC; aborting configure.")
endif()

add_compile_options(
    /W4            # Warning level 4
    # /WX            # Treat warnings as errors
    /permissive-   # Strict standards conformance
    /MP            # Multi-processor compilation
)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /sdl /analyze /guard:cf")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Oi /Ot /GL")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts OpenGL OpenGLWidgets)
find_package(GTest REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src/Model)
add_subdirectory(src/Presenter)
add_subdirectory(src/View)
add_subdirectory(Tests)

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

add_custom_command(TARGET StochasticSim POST_BUILD
  COMMAND "$<IF:$<CONFIG:Debug>,${VCPKG_INSTALLED_DIR}/x64-windows/tools/qt6/bin/windeployqt.debug.bat,${VCPKG_INSTALLED_DIR}/x64-windows/tools/qt6/bin/windeployqt.exe>"
          "$<TARGET_FILE:StochasticSim>"
          "--$<LOWER_CASE:$<CONFIG>>"
  COMMENT "Deploying Qt runtime"
)