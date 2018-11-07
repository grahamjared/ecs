#pragma once

#include "ecs/world.h"
#include "ecs/keychain.h"

#include "system_list.h"
#include "physics/components/position.h"
#include "physics/components/velocity.h"

using world    = ecs::world<system_list, position, velocity>;
using keychains = ecs::keychains<system_list>;
