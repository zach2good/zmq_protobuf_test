cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

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
    GITHUB_REPOSITORY madler/zlib
    VERSION 1.3.1
) # defines: zlibstatic

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

set(ABSL_PROPAGATE_CXX_STD ON)
set(protobuf_MODULE_COMPATIBLE ON)
set(protobuf_MSVC_STATIC_RUNTIME OFF)
set(protobuf_BUILD_SHARED_LIBS OFF)
set(protobuf_SHARED_OR_STATIC "STATIC")
CPMAddPackage(
    GITHUB_REPOSITORY protocolbuffers/protobuf
    GIT_TAG 439c42c735ae1efed57ab7771986f2a3c0b99319 # v28.0
    OPTIONS
        "protobuf_BUILD_TESTS NO"
)

# Protobuf-generated files
set(PROTO_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/proto/message.pb.cc)
set(PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/proto/message.pb.h)
add_custom_command(
    OUTPUT ${PROTO_SRCS} ${PROTO_HDRS}
    COMMAND ${CMAKE_BINARY_DIR}/_deps/protobuf-build/Debug/protoc.exe
    --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}
    --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/message.proto
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/proto/message.proto ${CMAKE_BINARY_DIR}/_deps/protobuf-build/Debug/protoc.exe
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

message(STATUS "PROTO_SRCS: ${PROTO_SRCS}")
message(STATUS "PROTO_HDRS: ${PROTO_HDRS}")

add_executable(server ${PROTO_SRCS} ${PROTO_HDRS} ${SOURCES})
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# server depends on protoc
add_dependencies(server protoc)

set(LIBS
    spdlog
    zlibstatic
    libzmq-static
    cppzmq-static
    protobuf::libprotobuf
)

target_compile_definitions(server PUBLIC _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
target_link_libraries(server PUBLIC ${LIBS})
