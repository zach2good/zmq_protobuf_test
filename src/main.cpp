#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <chrono>
using namespace std::chrono;

int main() {
  zmq::context_t ctx;
  zmq::socket_t sock(ctx, zmq::socket_type::router);
  sock.bind("tcp://127.0.0.1:6666");
  return 0;
}
