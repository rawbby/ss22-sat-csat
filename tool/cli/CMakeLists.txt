cmake_minimum_required(VERSION 3.1)

set(TARGET_NAME csatcli)

file(GLOB_RECURSE TARGET_SOURCES ./src/*.cpp)
file(GLOB_RECURSE TARGET_HEADER ./src/*.hpp)

add_executable(${TARGET_NAME})
target_link_libraries(${TARGET_NAME} csat)
target_sources(${TARGET_NAME} PRIVATE ${TARGET_SOURCES} ${TARGET_HEADER})

set_target_properties("${TARGET_NAME}" PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
