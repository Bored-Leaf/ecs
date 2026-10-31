#pragma once

#include <print>
#include <cassert>
#include <limits>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <utility>

#include "details/entity.hpp"

namespace ecs {
    /**
     * @brief A sparse set design for the ecs.
     *
     * @details The first version of this ecs uses a Sparse set data structure.
     * 
     * @tparam T The component of the sparse set.
     */
    template <typename T>
    class SparseSet {
    public:
        /**
         * @brief Checks if an @ref Entity has a component in the sparse set.
         * 
         * @param e The @ref Entity checked to have a component.
         * @return true If the entity has a component.
         * @return false If the sparse array size is smaller than the entities index
         * or if the entity does not have a component.
         */
        bool has(const ecs::Entity e) const {
            if (sparse.size() <= e.index()) {
                return false;
            }

            return sparse[e.index()] != kEmpty;
        }

        /**
         * @brief Adds an @ref Entity and component if the @ref Entity does have a component.
         *
         * @note Crashes when an @ref Entity with a component is passed to enforce strict state 
         * determinism and prevent unnoticed bugs.
         * 
         * @param e The @ref Entity to add.
         * @param component The component to add for e.
         */
        void add(const ecs::Entity e, const T& component) {
            if (has(e)) [[unlikely]] {
                // TODO: Use log callback for the print message for custom logging
                std::println(stderr, "sparseSet<T>::add(): The entity must not already contain a component. Entity Index: {}, Entity Generation: {}", e.index(), e.generation());
                std::abort();
            }
            
            if (sparse.size() <= e.index()) {
                sparse.resize(e.index() + 1, kEmpty);
            }

            std::uint32_t sparseIndex{static_cast<std::uint32_t>(denseEntities.size())};

            denseEntities.push_back(e);
            denseComponents.push_back(component);

            sparse[e.index()] = sparseIndex;
        }

        /**
         * @brief Removes an @ref Entity and its components if the @ref Entity has a component.
         *
         * @note Crashes when an @ref Entity without a component is passed to enforce strict
         * state determinism and prevent unnoticed bugs.
         * 
         * @param e The @ref Entity to remove.
         */
        void remove(const ecs::Entity e) {
            if (!has(e)) [[unlikely]] {
                std::println(stderr, "sparseSet<T>::remove(): The entity must have a component. Entity Index: {}, Entity Generation: {}", e.index(), e.generation());
                std::abort();
            }

            std::uint32_t densePosition{sparse[e.index()]};

            denseEntities[densePosition] = std::move(denseEntities.back());
            std::uint32_t swappedEntityDensePosition{denseEntities[densePosition].index()};
            sparse[swappedEntityDensePosition] = densePosition;
            denseEntities.pop_back();

            if (densePosition != denseComponents.size() - 1) {
                denseComponents[densePosition] = std::move(denseComponents.back());
            }
            denseComponents.pop_back();

            sparse[e.index()] = kEmpty;
        }

    private:
        /**
         * @internal
         * @brief The sparse array to hold entities.
         */
        std::vector<std::uint32_t>          sparse;

        /**
         * @internal
         * @brief The dense array to hold components.
         */
        std::vector<T>                      denseComponents;

        /**
         * @internal
         * @brief The dense array to hold @ref Entity.
         */
        std::vector<ecs::Entity>            denseEntities;

        /**
         * @internal
         * @brief Represents an empty slot in the sparse array.
         */
        constexpr static std::uint32_t      kEmpty{std::numeric_limits<uint32_t>::max()};
    };
}