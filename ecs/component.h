#pragma once

namespace ecs
{
    template <typename T, class empty>
    struct make_component : public T
    {
        using T::T;
        make_component() = default;
        make_component(const T & parent)
            : T(parent)
        {
        }

        make_component<T, empty> & operator=(const T & parent)
        {
            T::operator=(parent);
            return *this;
        }
    };
}
