#pragma once

namespace ecs
{
    template <class T, class empty>
    class make_component : public T
    {
    public:
        using T::T;
    };
}
