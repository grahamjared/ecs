#pragma once

#include "../world.h"

namespace rendering
{
    // declarations

    void print_positions(world & world);
    void print_velocities(world & world);

    // definitions

    void print_positions(world & world)
    {
        for (ecs::entity entity(0); entity < world.size(); ++entity)
        {
            // must unlock:
            // physics::POSITION && rendering::PRINT_POSITION
            const auto & keychain = world.keychain(entity);
            if (!keychain[system_list::physics].unlocks(physics::POSITION) ||
                !keychain[system_list::rendering].unlocks(rendering::PRINT_POSITION))
                continue;

            const auto & pos = world.get<position>(entity);
            std::cout << "entity #" << entity << " position: (" << pos.x << ", " << pos.y << ");\n";
        }
    }

    void print_velocities(world & world)
    {
        for (ecs::entity entity(0); entity < world.size(); ++entity)
        {
            // must unlock:
            // physics::VELOCITY && rendering::PRINT_VELOCITY
            const auto & keychain = world.keychain(entity);
            if (!keychain[system_list::physics].unlocks(physics::VELOCITY) ||
                !keychain[system_list::rendering].unlocks(rendering::PRINT_VELOCITY))
                continue;

            const auto & vel = world.get<velocity>(entity);
            std::cout << "entity #" << entity << " velocity: (" << vel.x << ", " << vel.y << ");\n";
        }
    }
}
