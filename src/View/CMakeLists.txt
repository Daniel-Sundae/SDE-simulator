set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Widgets Charts REQUIRED)

file(GLOB_RECURSE VIEW_SOURCES "*.cpp")
file(GLOB_RECURSE VIEW_HEADERS "*.hpp")

add_library(View ${VIEW_SOURCES} ${VIEW_HEADERS})

target_include_directories(View
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} Outputs Inputs
)

target_link_libraries(View
    PUBLIC Model Presenter CommonIncludes Qt6::Core Qt6::Widgets Qt6::Charts
)