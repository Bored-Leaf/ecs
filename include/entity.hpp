#pragma once

#include <cstdint>

namespace ecs {
    struct Entity {
        std::uint32_t indexGeneration{};

        static constexpr int            kIndexBits          = 16;
        static constexpr int            kGenerationBits     = 16;
        static constexpr std::uint32_t  kIndexMask          = (1u << kIndexBits) - 1;

        static_assert(kIndexBits + kGenerationBits == 32 && "kIndexBits + kGenerationBits cannot exceed 32 bits");

        constexpr std::uint32_t Index() const { return indexGeneration & kIndexMask; }
        constexpr std::uint32_t Generation() const { return indexGeneration & kIndexMask >> 16; }
        constexpr static Entity Make(std::uint32_t index, std::uint32_t generation)  { return Entity{.indexGeneration = generation << 16 | index}; }
    };

    struct EntityHandle {
        std::uint32_t indexGeneration{};
    };
}