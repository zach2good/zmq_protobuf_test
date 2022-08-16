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

```sh
./world_server &
./map_server &
```

# Tested on

- WSL + Ubuntu 22.04
