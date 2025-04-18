# Qt discovery and AUTOMOC/RCC/UIC are now handled globally in the root CMakeLists.txt

file(GLOB_RECURSE VIEW_SOURCES "*.cpp")
file(GLOB_RECURSE VIEW_HEADERS "*.hpp")

add_library(View ${VIEW_SOURCES} ${VIEW_HEADERS})

target_include_directories(View
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} # src/View
        Outputs                     # src/View/Outputs
        Inputs                      # src/View/Inputs
        ${CMAKE_SOURCE_DIR}/src/Presenter # Add Presenter include path
    INTERFACE
        ${Qt6Core_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
        ${Qt6Charts_INCLUDE_DIRS}
)

target_link_libraries(View
    PUBLIC Model Presenter CommonIncludes Qt6::Core Qt6::Widgets Qt6::Charts
)
