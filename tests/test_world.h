#pragma once

#include <iostream>
#include "examples/basic/world.h"

namespace test
{
    void world_copy()
    {
        ::world w;

        auto entity = w.create();
        w.keychain(entity)[system_list::physics].set(physics::POSITION);

        auto copy   = w.copy(entity);
        if (w.keychain(copy) == w.keychain(entity))
            std::cout << "world::copy() PASSED: keychain copy was good\n";
        else
            std::cout << "world::copy() FAILED: keychain was not copied\n";
    }

    void world()
    {
        world_copy();
    }
}