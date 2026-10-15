#pragma once

#include <cstdint>

namespace ecs {

    /**
     * @brief An Entity Handle to be handed back into the ecs.
     */
    struct EntityHandle {
        std::uint32_t indexGeneration{};    ///< Packed values of the EntityHandle's index and generation.

        /**
         * @brief Compares two entities for equality of their indexGeneration.
         * 
         * @param h The EntityHandle to compare to.
         * @return true if h.indexGeneration equals indexGeneration.
         * @return false if h.indexGeneration does not equal indexGeneration.
         */
        constexpr bool operator==(const EntityHandle& h) const {
            return h.indexGeneration == indexGeneration;
        }
    };
}