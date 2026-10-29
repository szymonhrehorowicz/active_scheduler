cmake_minimum_required(VERSION 3.22)

project(active_library)

# Dependencies
if(NOT TARGET etl)
    add_subdirectory(
        "${CMAKE_CURRENT_SOURCE_DIR}/external/etl"
        "${CMAKE_CURRENT_BINARY_DIR}/etl"
    )
endif()

add_library(active_library INTERFACE)

target_include_directories(active_library INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/incl")

target_compile_definitions(
    active_library
    INTERFACE
    ETL_CPP17_SUPPORTED=1
)

target_link_libraries(
    active_library
    INTERFACE
    etl
)