cmake_minimum_required(VERSION 3.22)

project(ActiveLibrary)
add_library(${PROJECT_NAME} INTERFACE)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

enable_language(C CXX ASM)

target_compile_definitions(${PROJECT_NAME} INTERFACE
)

target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/etl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/incl
)

target_sources(${CMAKE_PROJECT_NAME} INTERFACE
)

target_link_directories(${PROJECT_NAME} INTERFACE
)

target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE etl)

