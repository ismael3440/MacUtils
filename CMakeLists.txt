cmake_minimum_required(VERSION 1.30)

set(PROJECT_NAME macutils)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)

include_directories(include/)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/errors.cpp
    src/utils.cpp
    src/help.cpp
    src/restore.cpp
    src/search.cpp
)
