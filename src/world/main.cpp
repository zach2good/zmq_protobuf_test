#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "char_entity_generated.h"

#include <unordered_map>

#include <chrono>
using namespace std::chrono;

#include "../entities/char_entity.h"

#include "../helpers/cereal_helpers.h"
#include "../helpers/flatbuffer_helpers.h"
#include "../helpers/zmq_helpers.h"

int main()
{
    // ZMQ Setup
    spdlog::info(fmt::format("WORLD: Setting up ZMQ"));
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::router);
    sock.bind("tcp://127.0.0.1:6666");

    // Char Data
    std::unordered_map<unsigned int, CharEntity> lookup;

    CharEntity entity;
    entity.id = 42;
    entity.name = "Testo";
    for (int i = 0; i < 1'0; ++i)
    {
        entity.char_vars[std::string("NAME_") + std::to_string(i)] = i;
    }
    lookup[entity.id] = entity;

    // Flatbuffer Setup
    flatbuffers::FlatBufferBuilder builder(4096);

    // Timing
    unsigned int counter = 0;
    high_resolution_clock::time_point last_update = high_resolution_clock::now();

    spdlog::info(fmt::format("WORLD: Listening..."));
    while(true)
    {
        try
        {
            std::vector<zmq::message_t> recv_msgs;
            const auto ret = zmq::recv_multipart(sock, std::back_inserter(recv_msgs), zmq::recv_flags::dontwait);
            if (!ret.has_value() || ret.value() == 0)
            {
                continue;
            }

            unsigned int lookup_id = zmq_helpers::message_to_type<unsigned int>(recv_msgs[1]);
            auto looked_up_entity = lookup[lookup_id];
            auto span = looked_up_entity.to_packed_span(builder);
            recv_msgs[1] = zmq::message_t(span.data(), span.size());

            // Send over the network
            if (!zmq::send_multipart(sock, recv_msgs))
            {
                return 1;
            }

            ++counter;

            auto now = high_resolution_clock::now();
            if (now - last_update > seconds(1))
            {
                spdlog::info(fmt::format("Reqs served/sec: {}", counter));
                last_update = now;
                counter = 0;
            }
        }
        catch (zmq::error_t& e)
        {
            // Context was terminated
            // Exit loop
            spdlog::error(fmt::format("ZMQERR: {}", e.what()));
            if (!sock || e.num() == 156384765)
            {
                return 1;
            }

            continue;
        }
    }

    return 0;
}
