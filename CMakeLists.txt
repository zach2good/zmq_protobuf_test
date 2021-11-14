cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(mmo_test)

include(cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY gabime/spdlog
  VERSION 1.9.2
  OPTIONS "SPDLOG_BUILD_EXAMPLE NO" "SPDLOG_BUILD_TESTS NO"
) # defines: spdlog

CPMAddPackage(
  GITHUB_REPOSITORY zeromq/libzmq
  VERSION 4.3.4
  OPTIONS "BUILD_TESTS NO" "ZMQ_BUILD_TESTS NO"
) # defines: libzmq-static

CPMAddPackage(
  GITHUB_REPOSITORY zeromq/cppzmq
  VERSION 4.8.1
  OPTIONS "CPPZMQ_BUILD_TESTS NO"
) # defines: cppzmq-static

CPMAddPackage(
  GITHUB_REPOSITORY google/flatbuffers
  VERSION 2.0.0
  OPTIONS "FLATBUFFERS_BUILD_TESTS NO" "FLATBUFFERS_BUILD_GRPCTEST NO"
) # defines: flatbuffers

set(FLATBUFFER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/fbs/char_entity.fbs)

flatbuffers_generate_headers(
    TARGET generated_fbs
    SCHEMAS ${FLATBUFFER_SOURCES}
    #FLAGS --gen-object-api
)

set(SOURCES
    src/main.cpp)

add_executable(mmo_test ${SOURCES})
target_link_libraries(mmo_test PUBLIC spdlog libzmq-static cppzmq-static flatbuffers generated_fbs)
