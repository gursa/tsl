# Specify the minimum version.
cmake_minimum_required(VERSION 3.9)

# Specify the project info.
project(tsl_test VERSION 1.0.0 DESCRIPTION "Test app for tsl" LANGUAGES C)

set (CMAKE_C_STANDARD 99)

add_executable(${PROJECT_NAME}
    source/main.c
)

target_link_libraries (${PROJECT_NAME} LINK_PUBLIC tsl)
