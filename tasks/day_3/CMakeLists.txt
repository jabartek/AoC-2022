project(AoC-2022-day_3)

cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/)
set(HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/)

set(PROJECT_SOURCES
    ${SOURCE_DIR}/main.cc
)

add_executable(${PROJECT_NAME}
   ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE AoC-library)