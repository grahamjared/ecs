#pragma once

#include "ecs/component.h"
#include "vec2.h"

using velocity = ecs::make_component<vec2, struct _velocity>;
