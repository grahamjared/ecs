#pragma once

#include "ecs/world.h"
#include "ecs/keychain.h"

#include "system_list.h"
#include "physics/components/position.h"
#include "physics/components/velocity.h"

template <class system_list, class ... Ts>
class _world : public ecs::world<system_list, Ts...>
{
public:
    int value = 0;
};

using world = _world<system_list, position, velocity>;
using keychains = ecs::keychains<system_list>;
