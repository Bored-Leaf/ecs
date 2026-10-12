#pragma once

#include <cstdint>

namespace ecs {

    /**
     * @brief An Entity Handle to be handed back into the ecs.
     * 
     */
    struct EntityHandle {
        std::uint32_t indexGeneration{};

        constexpr bool operator==(const EntityHandle& e) const {
            return e.indexGeneration == indexGeneration;
        }
    };
}