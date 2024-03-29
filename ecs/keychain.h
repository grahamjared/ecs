#pragma once

#include <array>
#include <type_traits>

namespace ecs
{
    namespace help
    {
        template <class T, class = int >
        struct has_member_size : std::false_type { };

        template <class T>
        struct has_member_size<T, decltype((void) T::size, 0)> : std::true_type { };

        template <class T>
        bool constexpr has_member_size_v = has_member_size<T>::value;
    }

    class key
    {
    public:
        using component_t = unsigned int;

        inline bool unlocks(component_t lock) const noexcept
        {
            return (m_components & lock) == lock;
        }

        inline bool empty() const noexcept
        {
            return m_components == 0;
        }

        inline void set(component_t components) noexcept
        {
            m_components = components;
        }

        inline void add(component_t components) noexcept
        {
            m_components |= components;
        }

        inline void sub(component_t components) noexcept
        {
            m_components ^= (m_components & components);
        }

        inline component_t get() const noexcept
        {
            return m_components;
        }

        inline void clear() noexcept
        {
            m_components = 0;
        }

    private:
        component_t m_components = 0;
    };

    template <class system_list>
    class keychain
    {
    private:
        static_assert(std::is_enum_v<system_list>, "template parameter <system_list> must be an enum");
        static_assert(help::has_member_size_v<system_list>, "template parameter <system_list> must contain a member 'size' as the last member in the enum");

    public:
        inline void operator += (const keychain & rhs)
        {
            for (size_t i = 0; i < size(); ++i)
                m_keys[i].add(rhs.m_keys[i].get());
        }

        inline void operator -= (const keychain & rhs)
        {
            for (size_t i = 0; i < size(); ++i)
                m_keys[i].sub(rhs.m_keys[i].get());
        }

        inline bool operator == (const keychain & rhs)
        {
            for (size_t i = 0; i < size(); ++i)
                if (m_keys[i].get() != rhs.m_keys[i].get())
                    return false;

            return true;
        }

        inline key & operator[](system_list system)
        {
            return m_keys[static_cast<size_t>(system)];
        }

        inline const key & operator[](system_list system) const
        {
            return m_keys[static_cast<size_t>(system)];
        }

        inline bool empty() const noexcept
        {
            for (auto & key : m_keys)
                if (!key.empty())
                    return false;

            return true;
        }

        inline void clear() noexcept
        {
            for (auto & key : m_keys)
                key.clear();
        }

        inline size_t size() const
        {
            return static_cast<size_t>(system_list::size);
        }

    private:
        std::array<key, static_cast<size_t>(system_list::size)> m_keys;
    };
}
