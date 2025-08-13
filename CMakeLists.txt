cmake_minimum_required(VERSION 3.28)

project(hello-grpc VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_COMPILE_WARNING_AS_ERROR)
  if (MSVC)
    add_compile_options(/W4 /WX)
  else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
  endif()
endif()

find_package(gRPC CONFIG REQUIRED)

add_library(proto proto/example.proto)
target_link_libraries(proto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
target_include_directories(proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET proto LANGUAGE cpp)
protobuf_generate(
  TARGET proto
  LANGUAGE grpc
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
  PLUGIN_OPTIONS "generate_mock_code=true"
)

add_library(client_objects OBJECT
 ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/client.cpp
)
target_include_directories(client_objects PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(client_objects PUBLIC proto)

add_library(server_objects OBJECT
 ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/server.cpp
)
target_include_directories(server_objects PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(server_objects PUBLIC proto)

add_executable(client
 ${CMAKE_CURRENT_SOURCE_DIR}/src/app/client.cpp
)
target_link_libraries(client PRIVATE client_objects)

add_executable(server
 ${CMAKE_CURRENT_SOURCE_DIR}/src/app/server.cpp
)
target_link_libraries(server PRIVATE server_objects)

find_package(GTest CONFIG REQUIRED COMPONENTS GTest GMock)

enable_testing()

add_executable (tests
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_server.cpp
)

target_link_libraries (tests
  GTest::gmock
  GTest::gtest_main
  client_objects
  server_objects
)

include(GoogleTest)
gtest_discover_tests(tests)
