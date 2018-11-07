#pragma once

#include "ecs/component.h"
#include "vec2.h"

using position = ecs::make_component<vec2, struct _position>;
