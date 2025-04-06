file(GLOB_RECURSE MODEL_SOURCES "*.cpp")
file(GLOB_RECURSE MODEL_HEADERS "*.hpp")

add_library(Model ${MODEL_SOURCES} ${MODEL_HEADERS})

target_include_directories(Model
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} Calculations
)

target_link_libraries(Model
    PUBLIC CommonIncludes
)