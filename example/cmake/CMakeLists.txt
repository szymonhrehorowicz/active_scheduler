cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME ActiveLibraryExample)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Copy compile_commands.json to root directory
add_custom_target(copy-compile-commands ALL
    ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/../../compile_commands.json
    DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM CXX)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add ETL
add_subdirectory(${CMAKE_SOURCE_DIR}/../../ ActiveLibrary)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ../example.cpp
    ../interrupt.cpp

    ../motor_control/motor_control_task.cpp
    ../motor_control/motor_control_adapter.cpp

    ../pedal_position/pedal_position_task.cpp
    ../pedal_position/pedal_position_adapter.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../external/etl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    ActiveLibrary
)