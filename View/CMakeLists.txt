# View/CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(ViewModule)

# Find the required Qt components specific to View.
find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)

# Create a library for the view components. This library will compile all
# files in View, and optionally in its subdirectories.
add_library(ViewLib STATIC
    MainWindow.cpp
    MainWindow.hpp
    # Optionally add other view-related sources.
)

# If ChartManager is part of the view then you can add it either here or
# in an included subdirectory. For a separate subcomponent structure, consider:
# add_subdirectory(Charts)

# Link Qt libraries to the view library. Use PUBLIC if consumer targets should
# be aware of Qt headers, or PRIVATE if you want to hide Qt from consumers.
target_link_libraries(ViewLib PUBLIC Qt6::Widgets Qt6::Charts)

# Add the current directory to the include path for this module.
target_include_directories(ViewLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
