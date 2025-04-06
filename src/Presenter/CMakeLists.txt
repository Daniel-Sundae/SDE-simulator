file(GLOB_RECURSE PRESENTER_SOURCES "*.cpp")
file(GLOB_RECURSE PRESENTER_HEADERS "*.hpp")
file(GLOB_RECURSE PRESENTER_HEADERS "*.inl")

add_library(Presenter ${PRESENTER_SOURCES} ${PRESENTER_HEADERS})

target_include_directories(Presenter
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Presenter
    PUBLIC Model View CommonIncludes
)