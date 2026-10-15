#pragma once

#include <limits>
#include <vector>

template <typename T>
class SparseSet {

private:
    std::vector<std::uint32_t>          sparse;
    std::vector<T>                      denseComponentData;
    std::vector<std::uint32_t>          denseEntityId;
    constexpr static std::uint32_t      kempty{std::numeric_limits<uint32_t>::max()};
};