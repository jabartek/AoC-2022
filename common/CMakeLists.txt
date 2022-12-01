cmake_minimum_required(VERSION 3.24)
project(AoC-library CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/)
set(HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/)

set(PROJECT_SOURCES
    ${SOURCE_DIR}/arg_to_vec.cc
    ${SOURCE_DIR}/file_loader.cc
)

add_library(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${HEADER_DIR} 
)