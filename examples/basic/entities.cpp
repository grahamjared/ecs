#include "entities.h"

namespace spawn
{
    namespace add
    {
        void position(world & world, ecs::entity entity, const ::position & pos)
        {
            world.keychains()[entity][system_list::physics].add(physics::POSITION);
            world.get<::position>()[entity] = pos;
        }

        void velocity(world & world, ecs::entity entity, const ::velocity & vel)
        {
            world.keychains()[entity][system_list::physics].add(physics::VELOCITY);
            world.get<::velocity>()[entity] = vel;
        }

        void print_position(world & world, ecs::entity entity)
        {
            world.keychains()[entity][system_list::rendering].add(rendering::PRINT_POSITION);
        }

        void print_velocity(world & world, ecs::entity entity)
        {
            world.keychains()[entity][system_list::rendering].add(rendering::PRINT_VELOCITY);
        }
    }

    ecs::entity spawn::position_and_velocity(world & world, const ::position & pos, const ::velocity & vel)
    {
        auto entity = world.create();

        add::position(world, entity, pos);
        add::velocity(world, entity, vel);

        return entity;
    }

    ecs::entity spawn::position_and_velocity_with_printing(world & world, const ::position & pos, const ::velocity & vel)
    {
        auto entity = position_and_velocity(world, pos, vel);

        add::print_position(world, entity);
        add::print_velocity(world, entity);

        return entity;
    }
}
