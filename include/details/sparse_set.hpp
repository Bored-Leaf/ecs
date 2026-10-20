#pragma once

#include <limits>
#include <vector>
#include <cstdint>

#include "details/entity.hpp"

namespace ecs {

    template <typename T>
    class SparseSet {

    bool has(ecs::Entity e) const {
        if (sparse.size() <= e.index()) {
            return false;
        }

        return sparse[e.index()] != kEmpty;
    }

    void add(const ecs::Entity e, const T& component) {

    }

    private:
        std::vector<std::uint32_t>          sparse;
        std::vector<T>                      denseComponents;
        std::vector<ecs::Entity>            denseEntities;
        constexpr static std::uint32_t      kEmpty{std::numeric_limits<uint32_t>::max()};
    };
}