# Basic CMake gRPC project structure with tests

This is a minimal Protobuf/gRPC C++ example project using CMake with GTest and
GMock unit tests.

## Application details

The application is composed of a client and a server. Each of them have their
reusable components living under the `src/lib` folder. The application code
lives in `src/app`.

The client app runs a loop prompting strings from the user, the string "q" exits
the client.

The client will print an error on failed requests. Here is an example of failure
when the server is not running (or the client has not yet connected).

```bash
(q: exit) > abaco
Client: Error: the requesst has failed with error code: 14
```

A successful request should look like this in both client and server:
```bash
--------------------------------------------------------------------------------
| CLIENT STANDARD INPUT/OUTPUT          | SERVER STANDARD OUTPUT               |
| ------------------------------------- | ------------------------------------ |
| (q: exit) > five                      | Server: request for: [five]          |
| Client: char count of [five] is:4     |                                      |
--------------------------------------------------------------------------------
```

### Message specs

The basic application is a service that count characters of a given word. It is
defined in proto 3 syntax like this:

```proto
syntax = "proto3";

package example;

message Number { uint32 content = 1; }

message Text { string content = 1; }

service Char {
  rpc Count(Text) returns(Number) {}
}
```

When receiving a `Text` message, a `Number` will be sent as response.

### Client basic usage

The `grpc::Status CharClient::count(const std::string&, uint32_t&)` function
sends a `Text` containing the content of the provided string and process the
returned `Number`, storing the result in the output parameter *if the operation
succeeded*, which can be checked against the `grpc::Status::OK`.

```c++
  CharClient client("localhost:50001");
  uint32_t result;
  if (client.count("avocado", result) == grpc::Status::OK) {
    // do something with result
  }
```

### Server basic usage


```c++
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:50001", grpc::InsecureServerCredentials());

  CharService charService;
  builder.RegisterService(&charService);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();
```


## Considerations on CMake

* C++ standard is set to 17 without extensions
* Conditional warnings as errors enabled when CMAKE_COMPILE_WARNING_AS_ERROR has
been defined
* Use libraries objects to build client/server/tests

### Protobuf/gRPC

Using the system installed version.

```cmake
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

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
```

### Considerations on GTest

Using system installed version.

```cmake
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
```

## Unit test with Stub Mock

Protobuf can automatically create the mock class implementation of the service
stub interface. This has been done using 
`PLUGIN_OPTIONS "generate_mock_code=true"` in our `CMakeLists.txt`.

With the `MockCharStub` available, we can write the unit test with GTest like
this:

```c++
TEST(gRpcServerTest, StartsWithNoConnections)
{
  auto stub = std::make_unique<MockCharStub>();

  Number resp;
  resp.set_content(4);

  EXPECT_CALL(*stub, Count(_, _, _))
    .Times(AtLeast(1))
    .WillOnce(DoAll(SetArgPointee<2>(resp), Return(Status::OK)));

  CharClient client(std::move(stub));

  uint32_t count;
  auto status = client.count("test", count);
  EXPECT_TRUE(status.ok());
  EXPECT_EQ(count, 4);
}
```

## Debian/Ubuntu Requirements

```bash
sudo apt install cmake ninja-build clang protobuf-compiler-grpc libgrpc++-dev \
    libgtest-dev libgmock-dev
```

## Building

```bash
cmake --workflow release
```

## Running

Open two different shell sessions and change to `basic-grpc/build/release` on
each of them and run `./server` in one and `./client` on the other.
