cmake_minimum_required(VERSION 3.22)

project(ActiveLibrary)
add_library(${PROJECT_NAME} STATIC
    ${CMAKE_CURRENT_LIST_DIR}/src/dummy.cpp
)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

enable_language(C CXX ASM)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/etl etl)

target_compile_definitions(${PROJECT_NAME} PRIVATE
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/external/etl/include
    ${CMAKE_CURRENT_LIST_DIR}/incl
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
)

target_link_directories(${PROJECT_NAME} PRIVATE
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE etl)

