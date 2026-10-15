#pragma once

#include <cassert>
#include <cstdint>

#include "entity_handle.hpp"

namespace ecs{
    /**
        * @brief An entity that represents the index and generation.
        */
    struct Entity {
        std::uint32_t indexGeneration{}; ///< Packed values of the Entity's index and generation.

        constexpr static int            kIndexBits          = 16;                           ///< How many bits are assigned to the index.
        constexpr static int            kGenerationBits     = 16;                           ///< How many bits are assigned to the generation.
        constexpr static std::uint32_t  kIndexMask          = (1u << kIndexBits) - 1;       ///< Bitmask for the index bits.
        constexpr static std::uint32_t  kGenerationMask     = (1u << kGenerationBits) - 1;  ///< Bitmask for the generation bits.

        static_assert(kIndexBits + kGenerationBits == 32, "kIndexBits + kGenerationBits must be 32 bits");

        /**
         * @brief Returns the index value from @ref indexGeneration.
         * 
         * @return The Entity's index.
         */
        constexpr std::uint32_t Index()         const { return indexGeneration & kIndexMask; }

        /**
         * @brief Returns the generation value from @ref indexGeneration.
         * 
         * @return The Entity's generation. 
         */
        constexpr std::uint32_t Generation()    const { return indexGeneration >> kIndexBits; }

        /**
         * @brief Creates an @ref Entity via an index and generation.
         *
         * @pre index <= kIndexMask, generation <= kGenerationMask.
         * 
         * @param index The entity index.
         * @param generation The generation of the entity.
         * @return constexpr Entity An entity.
         */
        constexpr static Entity Make(std::uint32_t index, std::uint32_t generation) { 
            assert(index <= kIndexMask && "Make: index exceeds kIndexMask");
            assert(generation <= kGenerationMask && "Make: generation exceeds kGenerationMask");
            return Entity{.indexGeneration = generation << kIndexBits | index}; 
        }

        /**
         * @brief Converts an Entity to an Entity Handle.
         * 
         * @return The converted EntityHandle.
         */
        constexpr EntityHandle  ToHandle() const { return EntityHandle{.indexGeneration = indexGeneration}; }

        /**
         * @brief Converts an EntityHandle to an Entity.
         * 
         * @param h The EntityHandle converting to Entity.
         * @return The converted Entity.
         */
        constexpr static Entity FromHandle(const EntityHandle& h) { return Entity{.indexGeneration = h.indexGeneration}; }

        /**
         * @brief Checks if the generation is valid.
         * 
         * @return true if generation is larger than 0.
         * @return false if generation equals 0.
         */
        constexpr bool IsNull() const { return Generation() == 0; }

        /**
         * @brief Compares two entities for equality of their indexGeneration.
         * 
         * @param e The Entity to compare to.
         * @return true if e.indexGeneration equals indexGeneration.
         * @return false if e.indexGeneration does not equal indexGeneration.
         */
        constexpr bool operator==(const Entity& e) const {
            return e.indexGeneration == indexGeneration;
        }
    };  
}