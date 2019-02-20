#pragma once

#include "world.h"

namespace spawn
{
    // make sure the entity passed is valid before attempting to add components
    namespace add
    {
        void position(world & world, ecs::entity entity, const ::position & pos);
        void velocity(world & world, ecs::entity entity, const ::velocity & vel);
        void print_position(world & world, ecs::entity entity);
        void print_velocity(world & world, ecs::entity entity);
    }

    ecs::entity position_and_velocity(world & world, const ::position & pos, const ::velocity & vel);
    ecs::entity position_and_velocity_with_printing(world & world, const ::position & pos, const ::velocity & vel);
}
