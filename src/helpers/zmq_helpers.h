#pragma once

#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace zmq_helpers
{
    template <typename T>
    zmq::message_t type_to_message(T obj)
    {
        zmq::message_t message(sizeof(obj));
        std::memcpy(message.data(), &obj, sizeof(obj));
        return message;
    }

    template <typename T>
    T message_to_type(zmq::message_t const& msg)
    {
        T obj;
        std::memcpy(&obj, msg.data(), sizeof(T));
        return obj;
    }
};
