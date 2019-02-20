#include "physics.h"

namespace physics
{
    void update(world & world)
    {
        for (ecs::entity entity(0); entity < world.size(); ++entity)
        {
            const auto keychain = world.keychains()[entity];

            // physics system must unlock:
            // position && velocity
            if (!keychain[system_list::physics].unlocks(physics::POSITION | physics::VELOCITY))
                continue;

            auto & pos = world.get<position>()[entity];
            auto & vel = world.get<velocity>()[entity];

            pos += vel;
        }
    }
}
