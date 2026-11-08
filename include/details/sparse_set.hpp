#pragma once

#include <print>
#include <cassert>
#include <limits>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <utility>
#include <concepts>
#include <type_traits>

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
         * @brief Gets component data of `e`.
         * @details Gets the component data of an @ref Entity `e`, result is a reference, const if the
         * @ref SparseSet is const. 
         * If the result is a reference, component data can be updated in-place.
         *
         * @see replace() or addOrReplace() for updating whole components.
         * 
         * @note Crashes when an @ref Entity without a component is passed to enforce strict
         * state determinism and prevent unnoticed bugs.
         * @note Recommended to use `T` briefly: get it, use it and drop it within the same piece of code. Never 
         * store `T` long term, store the @ref EntityHandle instead and call @ref get again when you need the data.
         *
         * @warning `T` is only safe to use until the next @ref add, @ref replace, @ref addOrReplace or @ref remove call.
         *
         * @param e The @ref Entity to qurey.
         * @return Reference of the component data of `e`, const when @ref SparseSet is const.
         *
         * @see replace() or addOrReplace() for updating whole components.
         */
        auto& get(this auto& self, const ecs::Entity e) {
            if (!self.has(e)) [[unlikely]] {
                std::println(stderr, "SparseSet<T>::get(): The entity must have a component. Entity Index: {}, Entity Generation: {}", e.index(), e.generation());
                std::abort();
            }

            std::uint32_t densePosition{self.sparse[e.index()]};
            return self.denseComponents[densePosition];
        }

        /**
         * @brief Adds an @ref Entity and component if the @ref Entity does have a component.
         *
         * @note Crashes when an `e` already has a component to enforce strict state 
         * determinism and prevent unnoticed bugs.
         * 
         * @tparam U Component to add, same as `T`.
         * @param e The @ref Entity to add a component to.
         * @param component The component to add for `e`.
         */
        template <typename U>
            requires std::same_as<std::remove_cvref_t<U>, T>
        void add(const ecs::Entity e, U&& component) {
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
            denseComponents.push_back(std::forward<U>(component));

            sparse[e.index()] = sparseIndex;
        }

        /**
         * @brief Replaces component data of an @ref Entity if `e` already has a component.
         *
         * @note Crashes when an `e` doesn't have a component to enforce strict state
         * determinism and prevent unnoticed bugs.
         *
         * @tparam U Component to replace, same as `T`.
         * @param e The @ref Entity whose component to replace.
         * @param component The component to replace for `e`.
         *
         * @see get() for updating one or two fields of a component.
         */
        template <typename U>
            requires std::same_as<std::remove_cvref_t<U>, T>
        void replace(const ecs::Entity e, U&& component) {
            if (!has(e)) [[unlikely]] {
                std::println(stderr, "SparseSet<T>::replace(): The entity must have a component to replace. Entity Index: {}, Entity Generation: {}", e.index(), e.generation());
                std::abort();
            }

            get(e) = std::forward<U>(component);
        }

        /**
         * @brief 
         * 
         */
        void addOrReplace() {
        
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