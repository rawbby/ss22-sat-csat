cmake_minimum_required(VERSION 3.1)

set(TARGET_NAME csat)

file(GLOB_RECURSE TARGET_HEADER ./src/*.hpp)

add_library(${TARGET_NAME} INTERFACE include/csat/io.h)
target_sources(${TARGET_NAME} INTERFACE ${TARGET_HEADER})
target_include_directories(${TARGET_NAME} INTERFACE include)
set_target_properties("${TARGET_NAME}" PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
