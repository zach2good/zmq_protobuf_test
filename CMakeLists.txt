cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(mmo_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_GENERATOR_PLATFORM: ${CMAKE_GENERATOR_PLATFORM}")

include(cmake/CPM.cmake)

CPMAddPackage(
    GITHUB_REPOSITORY gabime/spdlog
    VERSION 1.9.2
    OPTIONS
        "SPDLOG_BUILD_EXAMPLE NO"
        "SPDLOG_BUILD_TESTS NO"
) # defines: spdlog

CPMAddPackage(
    GITHUB_REPOSITORY zeromq/libzmq
    VERSION 4.3.4
    OPTIONS
        "BUILD_TESTS NO"
        "ZMQ_BUILD_TESTS NO"
        "ENABLE_CURVE NO" # TODO: Fix me
) # defines: libzmq-static

CPMAddPackage(
    GITHUB_REPOSITORY zeromq/cppzmq
    VERSION 4.8.1
    OPTIONS
        "CPPZMQ_BUILD_TESTS NO"
) # defines: cppzmq-static

CPMAddPackage(
    GITHUB_REPOSITORY google/flatbuffers
    VERSION 2.0.0
    OPTIONS
        "FLATBUFFERS_BUILD_TESTS NO"
        "FLATBUFFERS_BUILD_GRPCTEST NO"
) # defines: flatbuffers

CPMAddPackage(
    NAME cereal
    VERSION 1.3.0
    GITHUB_REPOSITORY USCiLab/cereal
    OPTIONS
        "SKIP_PORTABILITY_TEST ON"
        "JUST_INSTALL_CEREAL ON"
)

set(FLATBUFFER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/schemas/char_entity.fbs
)

set(WORLD_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/world/main.cpp
)

set(MAP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/map/main.cpp
)

flatbuffers_generate_headers(
    TARGET generated_fbs
    SCHEMAS ${FLATBUFFER_SOURCES}
    #FLAGS --gen-object-api
)

set(LIBS
    spdlog
    libzmq-static
    cppzmq-static
    flatbuffers
    generated_fbs
    cereal
)

add_executable(world_server ${WORLD_SOURCES})
target_link_libraries(world_server PUBLIC ${LIBS})

add_executable(map_server ${MAP_SOURCES})
target_link_libraries(map_server PUBLIC ${LIBS})
