#pragma once

#include <cstdint>
#include <limits>
#include <vector>

#include "details/entity.hpp"

namespace ecs {
    /**
     * @brief Entity Manager for the ecs.
     *
     * @details Uses a free-list memory management pattern to recycle destroyed entity ID's without allocating new
     * memory.  
     * 
     */
    class EntityManager {

    private:
        struct EntitySlot {
            using StorageType = Entity::StorageType;
            
            // TODO: If Generation 0 is never met why Generation 0 exists as null?
            StorageType nextFreeGeneration{0x0000'0001};

            constexpr static int kNextFreeBits              {Entity::kIndexBits};
            constexpr static int kGenerationBits            {Entity::kGenerationBits};
            constexpr static StorageType kNextFreeMask      {Entity::kIndexMask};
            constexpr static StorageType kGenerationMask    {Entity::kGenerationMask};

            static_assert(kNextFreeBits + kGenerationBits == std::numeric_limits<StorageType>::digits, "kGenerationBits + kNextFreeBits must be StorageType::digits");
            static_assert(sizeof(StorageType) == sizeof(Entity::StorageType), "EntitySlots must match Entity byte size");
            static_assert(kNextFreeBits == Entity::kIndexBits, "Free-list index bits must match Entity index bits");
            static_assert(kGenerationBits == Entity::kGenerationBits, "EntitySlot Generation bits must match Entity generation bits");

            constexpr StorageType nextFreeIndex() const {
                return nextFreeGeneration >> kGenerationBits;
            }

            constexpr StorageType generation() const {
                return nextFreeGeneration & kGenerationMask;
            }

            constexpr void setNextFree(StorageType newNextFree) {
                nextFreeGeneration = (nextFreeGeneration & ~(kNextFreeMask << kGenerationBits))
                                   | ((newNextFree & kNextFreeMask) << kGenerationBits);
            }

            constexpr void setGeneration(StorageType newGen) {
                nextFreeGeneration = (nextFreeGeneration & ~kGenerationMask)
                                   | (newGen & kGenerationMask);
            }
        };

        std::vector<EntitySlot>         slots{};            ///< @internal Holds @ref EntitySlots 
        std::uint16_t                   head{kEmpty};       ///< @internal Holds the index of next reusable @ref Entity slot

        constexpr static std::uint16_t  kEmpty{std::numeric_limits<std::uint16_t>::max()};  ///< @internal Represents the end of reusable slots
        static_assert(kEmpty == Entity::kIndexMask, "kEmpty must be same bytes size as Entity::kIndexMask");
        
    };
}