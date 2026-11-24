#pragma once

#include <print>
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
    public:

        /** @internal
         * @brief 
         * 
         * @note If external code has kept an @ref EntityHandle with generation 1 at slot x for long enough that
         * it looped from 1 to max BACK to 1 then the @ref EntityManager will consider this generation valid. To avoid this
         * external code has the ability to specify the index and generation bits in @ref Entity.
         *
         * @return Entity 
         */
        Entity create() {
        // TODO: Entity Rework
            if (head != kEmpty) {
                EntitySlot& freeSlot{slots[head]};
                EntitySlot::StorageType freeSlotIndex{head};
                head = freeSlot.nextFreeIndex();

                freeSlot.setNextFree(kEmpty);
                
                return Entity::make(freeSlotIndex, freeSlot.generation());
            } else {
                if (slots.size() == kEmpty) [[unlikely]] {
                    std::println(stderr, "EntityManager::create(): The number of entities is already at max, cannot create another entity. Max slots is {}", slots.size());
                    std::abort();
                }
                
                Entity::StorageType entityIndex = slots.size();

                EntitySlot newEntitySlot;
                slots.push_back(newEntitySlot);

                return Entity::make(entityIndex, newEntitySlot.generation());
            }
        }

        void destroy() {
        
        }

    private:
        struct EntitySlot {
            using StorageType = Entity::StorageType;
            
            // TODO: If Generation 0 is never met why Generation 0 exists as null?
            /** @internal
             * @brief Packed values of the EntitySlots next free index and generation.
             * @details Defaults to kEmpty for nextFree and 1 for generation.
             */
             // TODO: Entity rework figure out the nextFree default.
            StorageType nextFreeGeneration{0xFFFF'0001};

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

            constexpr void bumpGeneration() {
                StorageType newGen = generation() + 1;
                if ((newGen & kGenerationMask) == 0) {
                    newGen = 1;
                }

                setGeneration(newGen);
            }

        private:
            constexpr void setGeneration(StorageType newGen) {
                nextFreeGeneration = (nextFreeGeneration & ~kGenerationMask)
                                   | (newGen & kGenerationMask);
            }
        };

        std::vector<EntitySlot>         slots{};            ///< @internal Holds @ref EntitySlots
        // TODO: Change when updating Entity to templated
        std::uint16_t                   head{kEmpty};       ///< @internal Holds the index of next reusable @ref Entity slot

        // TODO: Change when updating Entity to templated
        constexpr static std::uint16_t  kEmpty{std::numeric_limits<std::uint16_t>::max()};  ///< @internal Represents the end of reusable slots
        static_assert(kEmpty == Entity::kIndexMask, "kEmpty must be same bytes size as Entity::kIndexMask");
    };
}