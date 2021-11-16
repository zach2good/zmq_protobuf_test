#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "char_entity_generated.h"

#include <chrono>
using namespace std::chrono;

#include "../entities/char_entity.h"

#include "../helpers/cereal_helpers.h"
#include "../helpers/flatbuffer_helpers.h"
#include "../helpers/zmq_helpers.h"

int main()
{
    // ZMQ Setup
    spdlog::info(fmt::format("MAP: Setting up ZMQ"));
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::dealer);
    sock.connect("tcp://127.0.0.1:6666");

    while (true)
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        std::vector<zmq::message_t> send_msgs;
        send_msgs.emplace_back(zmq_helpers::type_to_message<unsigned int>(42));

        if (!zmq::send_multipart(sock, send_msgs))
        {
            return 1;
        }

        std::vector<zmq::message_t> recv_msgs;
        const auto ret = zmq::recv_multipart(sock, std::back_inserter(recv_msgs), zmq::recv_flags::dontwait);
        if (!ret.has_value())
        {
            continue;
        }

        CharEntity entity(recv_msgs[0]);

        spdlog::info(fmt::format("CharEntity.id: {} ", entity.id));
        spdlog::info(fmt::format("CharEntity.name: {} ", entity.name));
        spdlog::info(fmt::format("CharEntity.char_vars.size: {} ", entity.char_vars.size()));

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto time_span = duration_cast<microseconds>(t2 - t1);
        spdlog::info(fmt::format("Time taken: {}us ({}/second)", time_span.count(), 1'000'000 / time_span.count()));
    }
    return 0;
}
