#pragma once

#include <chrono>
#include <iostream>
#include <thread>

#include "entities.h"
#include "physics/physics.h"
#include "rendering/rendering.h"

namespace basic
{
    void run(int iterations)
    {
        world world;

        spawn::position_and_velocity(world, position(0, 0), velocity(0, 0));
        // or
        spawn::position_and_velocity(world, { 0, 0 }, { 0, -1 });

        spawn::position_and_velocity_with_printing(world, { 0, 0 }, { 0, 0 });
        spawn::position_and_velocity_with_printing(world, { 0, 0 }, { 1, 0 });
        spawn::position_and_velocity_with_printing(world, { -5, 12 }, { 2, -6 });

        for (auto i = 0; i < iterations; ++i)
        {
            std::cout << "starting iteration...\n\n";

            physics::update(world);
            rendering::print_positions(world);
            rendering::print_velocities(world);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "\n...end\n\n";
        }
    }
}
