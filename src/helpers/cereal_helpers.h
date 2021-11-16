#pragma once

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <sstream>

namespace cereal_helpers
{
    template <typename T>
    std::string serialize(T const& obj)
    {
        std::ostringstream oss;
        {
            cereal::BinaryOutputArchive archive(oss);
            archive(obj);
        }
        return oss.str();
    }

    template <typename T>
    T deserialize(std::string const& str)
    {
        T obj;
        {
            std::istringstream iss(str);
            cereal::BinaryInputArchive archive(iss);
            archive(obj);
        }
        return obj;
    }
};
