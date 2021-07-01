cmake_minimum_required(VERSION 3.0.0)
project(clox VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(clox main.c memory.c chunk.c debug.c value.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
