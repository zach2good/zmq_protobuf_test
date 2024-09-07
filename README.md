# ZMQ & Protocol Buffers Test

## Deps

- spdlog
- zlib
- libzmq(static)
- cppzmq(static)
- protobuf

## Build

```sh
mkdir build
cd build
cmake ..
cmake --build . -j8
```

NOTE: Build currently not working, but it takes _a long time_ just to build `absl` and `protoc`, so I'm not particularly willing to carry on with this.
