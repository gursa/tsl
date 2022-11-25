# Specify the minimum version.
cmake_minimum_required(VERSION 3.9)

# Specify the project info.
project(tsl VERSION 1.0.0 DESCRIPTION "Shared library with telnet server with debug functions" LANGUAGES C)

set (CMAKE_C_STANDARD 99)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Declare the library target.
add_library(${PROJECT_NAME} SHARED
    source/mem_dump.c
    source/mem_dump.h
    source/utils.c
    source/utils.h
    source/telnet_serv.c
    source/telnet_serv.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})

# Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PRIVATE source)

# Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

# Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER source/telnet_serv.h)


