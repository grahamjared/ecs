#pragma once

enum class system_list
{
    physics,
    rendering,

    size // always last
};

namespace physics
{
    enum
    {
        POSITION = 1 << 0, // make sure to do '1 << n'
        VELOCITY = 1 << 1, // make sure to do '1 << n+1' etc
    };
}

namespace rendering
{
    enum
    {
        PRINT_POSITION = 1 << 0,
        PRINT_VELOCITY = 1 << 1,
    };
}
