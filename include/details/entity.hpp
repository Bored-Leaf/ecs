#pragma once

#include <cstdint>

#include "entity_handle.hpp"

namespace ecs{
    /**
        * @brief An entity that represents the index and generation.
        * 
        */
    struct Entity {
        std::uint32_t indexGeneration{};

        constexpr static int            kIndexBits          = 16;
        constexpr static int            kGenerationBits     = 16;
        constexpr static std::uint32_t  kIndexMask          = (1u << kIndexBits) - 1;
        constexpr static std::uint32_t  kGenerationMask     = (1u << kGenerationBits) - 1;

        static_assert(kIndexBits + kGenerationBits == 32, "kIndexBits + kGenerationBits must be 32 bits");

        constexpr std::uint32_t Index()         const { return indexGeneration & kIndexMask; }
        constexpr std::uint32_t Generation()    const { return indexGeneration >> kIndexBits; }

        constexpr static Entity Make(std::uint32_t index, std::uint32_t generation) { return Entity{.indexGeneration = generation << kIndexBits | index}; }
        constexpr EntityHandle  ToHandle() const                                    { return EntityHandle{.indexGeneration = indexGeneration}; }
        constexpr static Entity FromHandle(const EntityHandle& h)                   { return Entity{.indexGeneration = h.indexGeneration}; }

        constexpr bool IsNull() const { return Generation() == 0; }

        constexpr bool operator==(const Entity& e) const {
            return e.indexGeneration == indexGeneration;
        }
    };  
}