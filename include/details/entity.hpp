#pragma once

#include <cassert>
#include <cstdint>
#include <limits>

#include "entity_handle.hpp"

namespace ecs{
    /** @internal
    * @brief An entity that represents the index and generation.
    */
    struct Entity {
        using StorageType = std::uint32_t;

        StorageType indexGeneration{}; ///< Packed values of the Entity's index and generation.

        constexpr static int            kIndexBits          {16};                           ///< @internal How many bits are assigned to the index.
        constexpr static int            kGenerationBits     {16};                           ///< @internal How many bits are assigned to the generation.
        constexpr static StorageType    kIndexMask          {(1u << kIndexBits) - 1};       ///< @internal Bitmask for the index bits.
        constexpr static StorageType    kGenerationMask     {(1u << kGenerationBits) - 1};  ///< @internal Bitmask for the generation bits.

        static_assert(kIndexBits + kGenerationBits == std::numeric_limits<StorageType>::digits, "kIndexBits + kGenerationBits must be StorageType::digits");

        /** @internal
         * @brief Returns the index value from @ref indexGeneration.
         * 
         * @return The Entity's index.
         */
        constexpr StorageType index()         const { return indexGeneration & kIndexMask; }

        /** @internal
         * @brief Returns the generation value from @ref indexGeneration.
         * 
         * @return The Entity's generation. 
         */
        constexpr StorageType generation()    const { return indexGeneration >> kIndexBits; }

        /** @internal
         * @brief Creates an @ref Entity via an index and generation.
         *
         * @pre index <= kIndexMask, generation <= kGenerationMask.
         * 
         * @param index The entity index.
         * @param generation The generation of the entity.
         * @return constexpr Entity An entity.
         */
        constexpr static Entity make(StorageType index, StorageType generation) { 
            assert(index <= kIndexMask && "Entity::Make: index exceeds kIndexMask");
            assert(generation <= kGenerationMask && "Entity::Make: generation exceeds kGenerationMask");
            return Entity{.indexGeneration = generation << kIndexBits | index}; 
        }

        /** @internal
         * @brief Converts an Entity to an Entity Handle.
         * 
         * @return The converted EntityHandle.
         */
        constexpr EntityHandle  toHandle() const { return EntityHandle{.indexGeneration = indexGeneration}; }

        /** @internal
         * @brief Converts an EntityHandle to an Entity.
         * 
         * @param h The EntityHandle converting to Entity.
         * @return The converted Entity.
         */
        constexpr static Entity fromHandle(const EntityHandle& h) { return Entity{.indexGeneration = h.indexGeneration}; }

        /** @internal
         * @brief Checks if the generation is valid.
         * 
         * @return true if generation is larger than 0.
         * @return false if generation equals 0.
         */
        constexpr bool isNull() const { return generation() == 0; }

        /** @internal
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