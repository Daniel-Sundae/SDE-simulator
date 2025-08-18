file(GLOB_RECURSE PRESENTER_SOURCES "*.cpp")
file(GLOB_RECURSE PRESENTER_HEADERS "*.hpp")

add_library(Presenter ${PRESENTER_SOURCES} ${PRESENTER_HEADERS})

# Include directories are often handled by target_link_libraries with Qt targets
target_include_directories(Presenter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        # Explicitly add Qt include directories for Intellisense
        ${Qt6Core_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Include
)

target_link_libraries(Presenter
    PUBLIC
        Model
        View
        Qt6::Core
)
