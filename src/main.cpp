// https://github.com/igorskh/flatbuffers-zmq-tutorial-cpp

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "char_entity_generated.h"

int main()
{
    // ZMQ Setup
    zmq::context_t ctx;
    zmq::socket_t sock1(ctx, zmq::socket_type::push);
    zmq::socket_t sock2(ctx, zmq::socket_type::pull);
    sock1.bind("tcp://127.0.0.1:*");
    const std::string last_endpoint =
        sock1.get(zmq::sockopt::last_endpoint);
    spdlog::info(fmt::format("Connecting to: {}", last_endpoint));
    sock2.connect(last_endpoint);

    // Sender
    {
        spdlog::info(fmt::format("Building flatbuffer"));
        flatbuffers::FlatBufferBuilder builder(1024);

        // Copy in data from in-memory object
        spdlog::info(fmt::format("Copying in data"));

        // Get data ready to be added to the builder's buffer
        auto name = builder.CreateString("Test name!");
        auto pos = FB::Vec3(10.0f, 20.0f, 30.0f);
        
        // Add the data now
        // NOTE: Not doing the preparing/packing in this order will trigger the 
        // very helpful flatbuffers::FlatBufferBuilder::NotNested()
        FB::CharEntityBuilder char_entity_builder(builder);
        char_entity_builder.add_name(name);
        char_entity_builder.add_pos(&pos);

        // Pack
        spdlog::info(fmt::format("Packing"));
        builder.Finish(char_entity_builder.Finish());

        // Use data bytes between ptr and size:
        auto* ptr = builder.GetBufferPointer();
        auto  sz  = builder.GetSize();

        std::array<zmq::const_buffer, 1> send_msgs = {
            zmq::buffer(ptr, sz)
        };

        // Send over the network
        spdlog::info(fmt::format("Sending messages"));
        if (!zmq::send_multipart(sock1, send_msgs))
        {
            return 1;
        }
    }

    // Receiver
    {
        spdlog::info(fmt::format("Recv messages"));
        std::vector<zmq::message_t> recv_msgs;
        const auto ret = zmq::recv_multipart(
            sock2, std::back_inserter(recv_msgs));

        if (!ret)
        {
            return 1;
        }

        spdlog::info(fmt::format("Got {} messages:", *ret));
        for (auto const& msg : recv_msgs)
        {
            auto* CharEntity = FB::GetCharEntity(msg.data());
            spdlog::info(fmt::format("CharEntity.name: {} ", CharEntity->name()->c_str()));
            spdlog::info(fmt::format("CharEntity.pos.x: {} ", CharEntity->pos()->x()));
            spdlog::info(fmt::format("CharEntity.pos.y: {} ", CharEntity->pos()->y()));
            spdlog::info(fmt::format("CharEntity.pos.z: {} ", CharEntity->pos()->z()));
        }
    }

    return 0;
}
