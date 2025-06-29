# Basic CMake gRPC project structure with tests

This is a minimal Protobuf/gRPC C++ example project using CMake with GTest and
GMock unit tests.

## Debian/Ubuntu Requirements

```
sudo apt install cmake ninja-build clang protobuf-compiler-grpc libgrpc++-dev \
    libgtest-dev libgmock-dev
```

## Building

```
cmake --workflow release
```

## Running

Open two different shell sessions and change to `basic-grpc/build/release` on
each of them and run `./server` in one and `./client` on the other.
