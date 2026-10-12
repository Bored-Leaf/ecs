#include <print>
#include "details/entity.hpp"

int main() {
    std::println("Bored Leaf's ECS Tests");

    std::println("Testing Bit manipulation's don't leak");
    static_assert(ecs::Entity::Make(5, 1).Generation() == 1);

    static_assert(ecs::Entity::Make(ecs::Entity::kIndexMask, 1).Index()  == ecs::Entity::kIndexMask);
    static_assert(ecs::Entity::Make(ecs::Entity::kIndexMask, 1).Generation() == 1);

    static_assert(ecs::Entity::Make(0, ecs::Entity::kGenerationMask).Index() == 0);
    static_assert(ecs::Entity::Make(0, ecs::Entity::kGenerationMask).Generation() == ecs::Entity::kGenerationMask);

    static_assert(ecs::Entity::Make(ecs::Entity::kIndexMask, ecs::Entity::kGenerationMask).indexGeneration == 0xFFFF'FFFF);

    static_assert(ecs::Entity::FromHandle(ecs::Entity::Make(5, 1).ToHandle()) == ecs::Entity::Make(5, 1));

    static_assert(ecs::Entity{}.IsNull() == true);
    static_assert(ecs::Entity::Make(0, 3).IsNull() == false);

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