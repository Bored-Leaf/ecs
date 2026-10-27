#include <print>
#include <cassert>

#include "details/entity.hpp"
#include "details/sparse_set.hpp"

struct Position {
    float x;
    float y;
    float z;
};

int main() {
    std::println("Bored Leaf's ECS Tests");

    std::println("-- Testing Bit manipulation's don't leak --");
    std::println("*Contains only static_asserts");
    static_assert(ecs::Entity::make(5, 1).generation() == 1);

    static_assert(ecs::Entity::make(ecs::Entity::kIndexMask, 1).index()  == ecs::Entity::kIndexMask);
    static_assert(ecs::Entity::make(ecs::Entity::kIndexMask, 1).generation() == 1);

    static_assert(ecs::Entity::make(0, ecs::Entity::kGenerationMask).index() == 0);
    static_assert(ecs::Entity::make(0, ecs::Entity::kGenerationMask).generation() == ecs::Entity::kGenerationMask);

    static_assert(ecs::Entity::make(ecs::Entity::kIndexMask, ecs::Entity::kGenerationMask).indexGeneration == 0xFFFF'FFFF);

    static_assert(ecs::Entity::fromHandle(ecs::Entity::make(5, 1).toHandle()) == ecs::Entity::make(5, 1));

    static_assert(ecs::Entity{}.isNull() == true);
    static_assert(ecs::Entity::make(0, 3).isNull() == false);

    std::println("-- Testing sparse set functionality --");
    ecs::SparseSet<Position> sparse;
    ecs::Entity entity{ecs::Entity::make(1, 1)};

    bool hasEmptySet{sparse.has(entity)};
    std::println("has() with Empty set, should be false: {}", hasEmptySet);

    entity = ecs::Entity::make(50, 1);
    bool indexBeyondSize{sparse.has(entity)};
    std::println("has() with entity index beyond sparse.size(), should be false: {}", indexBeyondSize);

    sparse.add(entity, Position(1, 1, 1));
    entity = ecs::Entity::make(25, 1);
    bool iskEmpty{sparse.has(entity)};
    std::println("index within sparse.size() but is kEmpty, should be false: {}", iskEmpty);

    entity = ecs::Entity::make(45, 2);
    sparse.add(entity, Position(2, 2, 2));
    bool hasComponent{sparse.has(entity)};
    std::println("entity with a component, should be true: {}", hasComponent);
    /*
        Tests overview
        Test components: 
            Postion
            Velocity
            User defined object. Have types that own data like std::string and std::vector. Count
                its copies and moves. Tests move semantics and lets Asan catch leaks and double frees.
                Destructor counter, destructor should only run once when removing component or entity.

        Test scene:
            Entity____|______Components
            e1        |      Position, Velocity, Custom
            e2        |      Position, Velocity 
            e3        |      Position, Custom
            e4        |      Velocity
            e5        |      None

        Tests cases
        1. kIndexBits and kGenerationBits: make a few entities and check
            that the index and generation come back unchanged, including the max index and generation.
        2. Add and get: Each added component can be read back with right values.
        3. View intersection:
              * Test scene not created
                View<Position, Velocity>    - expected gracefully returns nothing/empty iterator/empty range
              * Test scene created
                View<Position, Velocity>    - expected e1, e2
                View<Velocity, Position>    - expected e1, e2
                View<Position, Custom>      - expected e1, e3
                View<Velocity>              - expected e1, e2, e4
                View<Position, Velocity>    - expected nothing
            e5 never appears but is still alive.
        4. Swap remove (middle): Remove a components in the middle of the pool. The moved entity must still
            resolve its own data, the removed entity no longer has the component, the view result lose only
            that entity, and sparse and dense still agree.
        5. Swap remove (last): Needs no swap
        6. Move semantics: during swap remove the custom type is moved, not copied (check the counters), and
            ASan reports nothing.
        7. Generation, Handle Validity, and Destructor Lifecycle:
            - Add a Custom component to an entity.
            - Copy its handle (old_handle).
            - Destroy the entity. Assert that the Custom component's destructor runs exactly once here.
            - Assert that registry.has<Custom>(old_handle) is false and registry.is_alive(old_handle) is false.
            - Create a new entity (new_handle) that recycles the slot.
            - Assert old_handle and new_handle share the same index, but new_handle has a higher generation.
            - Assert old_handle is STILL not alive, and querying its components returns false/errors (stale handle check).
            - Assert new_handle IS alive, but does NOT inherit the old entity's Custom component.
            - Generation behavior: Ensure generation is never 0, and overflow/wrapping skips 0.
    */
}