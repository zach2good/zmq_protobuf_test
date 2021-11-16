# ZMQ & Flatbuffers Example

## Deps

- spdlog
- libzmq(static)
- cppzmq(static)
- flatbuffers
- cereal

## Build

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

## Run

```
./world_server &
./map_server &
```
