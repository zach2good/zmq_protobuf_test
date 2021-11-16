#pragma once

#include <string>
#include <unordered_map>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "char_entity_generated.h"

#include "../helpers/cereal_helpers.h"

struct CharEntity
{
    unsigned int id;
    std::string name;
    std::unordered_map<std::string, unsigned int> char_vars;

    CharEntity() = default;
    CharEntity(zmq::message_t const& msg)
    {
        auto* FBCharEntity = GetFBCharEntity(msg.data());

        this->id = FBCharEntity->id();
        this->name = FBCharEntity->name()->str();
        this->char_vars = cereal_helpers::deserialize<std::unordered_map<std::string, unsigned int>>(FBCharEntity->char_vars()->str());
    }

    flatbuffers::span<uint8_t> to_packed_span(flatbuffers::FlatBufferBuilder& builder)
    {
        builder.Clear();

        // Prepare structs
        auto id = this->id;
        auto name = builder.CreateString(this->name);
        auto char_vars = builder.CreateString(cereal_helpers::serialize(this->char_vars));

        // Start packing
        FBCharEntityBuilder char_entity_builder(builder);

        char_entity_builder.add_id(id);
        char_entity_builder.add_name(name);
        char_entity_builder.add_char_vars(char_vars);

        // Finish packing
        builder.Finish(char_entity_builder.Finish());

        // Use data bytes between ptr and size
        auto* ptr = builder.GetBufferPointer();
        auto  sz  = builder.GetSize();

        return flatbuffers::span<uint8_t>(ptr, sz);
    }
};
