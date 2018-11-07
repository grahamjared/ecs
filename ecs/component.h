#pragma once

namespace ecs
{
    template <class T, class empty>
    struct make_component : public T
    {
        using T::T;
    };
}
