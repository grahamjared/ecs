#pragma once

#include <vector>
#include <tuple>

#include "keychain.h"
#include "entity.h"

namespace ecs
{
    namespace detail
    {
        template <int ... Is>
        struct seq { };

        template <int N, int ... Is>
        struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

        template <int ... Is>
        struct gen_seq<0, Is...> : seq<Is...> { };

        template <class T, class F, int ... Is>
        void for_each(T && t, F f, seq<Is...>)
        {
            auto l = { (f(std::get<Is>(std::forward<T>(t))), 0)... };
        }
    }

    template <class ... Ts, class F>
    void for_each_in_tuple(std::tuple<Ts...> const & t, F f)
    {
        detail::for_each(t, f, detail::gen_seq<sizeof...(Ts)>());
    }

    template <class ... Ts, class F>
    void for_each_in_tuple(std::tuple<Ts...> & t, F f)
    {
        detail::for_each(t, f, detail::gen_seq<sizeof...(Ts)>());
    }

    template <class system_list, class ... Ts>
    class world
    {
    public:
        using vectors_t = std::tuple<std::vector<keychain<system_list>>, std::vector<Ts>... >;
        vectors_t m_components;
        world() = default;

        template<int N, typename T>
        struct vector_of_t : std::is_same<T,
            typename std::tuple_element<N, vectors_t>::type::value_type>
        { };

        template <int N, class T, class Tuple,
            bool Match = false>
            struct MatchingField
        {
            static std::vector<T> & get(Tuple & tp)
            {
                // The "non-matching" version
                return MatchingField<N + 1, T, Tuple,
                    vector_of_t<N + 1, T>::value>::get(tp);
            }

            static const std::vector<T> & get(const Tuple & tp)
            {
                // The "non-matching" version
                return MatchingField<N + 1, T, Tuple,
                    vector_of_t<N + 1, T>::value>::get(tp);
            }
        };

        template <int N, class T, class Tuple>
        struct MatchingField<N, T, Tuple, true>
        {
            static std::vector<T> & get(Tuple & tp)
            {
                return std::get<N>(tp);
            }

            static const std::vector<T> & get(const Tuple & tp)
            {
                return std::get<N>(tp);
            }
        };

    public:
        ////////////////////////////////////////////////////////////
        // Retrieves a vector<Ts> & (a vector of components) at the
        // index <I>
        ////////////////////////////////////////////////////////////
        template <class T>
        inline auto & get_component()
        {
            return MatchingField<0, T, vectors_t, vector_of_t<0, T>::value>::get(m_components);
        }

        template <class T>
        inline const auto & get_component() const
        {
            return MatchingField<0, T, vectors_t, vector_of_t<0, T>::value>::get(m_components);
        }

        ////////////////////////////////////////////////////////////
        // get an entity's component based on type
        ////////////////////////////////////////////////////////////
        template <class T>
        inline auto & get(ecs::entity entity)
        {
            return MatchingField<0, T, vectors_t, vector_of_t<0, T>::value>::get(m_components)[entity];
        }

        template <class T>
        inline const auto & get(ecs::entity entity) const
        {
            return MatchingField<0, T, vectors_t, vector_of_t<0, T>::value>::get(m_components)[entity];
        }

        inline auto & keychain(ecs::entity entity)
        {
            return get<ecs::keychain<system_list>>(entity);
        }

        ////////////////////////////////////////////////////////////
        // First tries to find a deleted entity (an entity with all keys empty) and repurpose that as a new entity
        // If that fails, it pushes back all components and returns the newly allocated entity
        ////////////////////////////////////////////////////////////
        inline entity create()
        {
            for (entity entity = 0; entity < size(); ++entity)
                if (get<ecs::keychain<system_list>>(entity).empty())
                    return entity;

            push_all();
            return size() - 1;
        }

        ////////////////////////////////////////////////////////////
        // All destroying an entity does is clear its keychain
        // This marks it as destroyed, so it can be repurposed without reallocating more memory
        ////////////////////////////////////////////////////////////
        inline void destroy(entity entity) noexcept
        {
            get<ecs::keychain<system_list>>(entity).clear();
        }

        ////////////////////////////////////////////////////////////
        // Tests if a entity is within the bounds of the world
        ////////////////////////////////////////////////////////////
        inline bool valid(entity entity) const noexcept
        {
            return (entity > -1 && entity < static_cast<int>(size()));
        }

        ////////////////////////////////////////////////////////////
        // Tests if an entity has at least one non-empty key
        // (aka still being used by a system)
        ////////////////////////////////////////////////////////////
        inline bool exists(entity entity) const noexcept
        {
            if (!valid(entity))
                return false;
            else
                return !get<ecs::keychain<system_list>>(entity).empty();
        }

        ////////////////////////////////////////////////////////////
        // Below are functions identical to std::vector:: functions
        // They work exactly the same, but operate on all vectors
        ////////////////////////////////////////////////////////////

        inline size_t capacity() noexcept
        {
            return get_component<ecs::keychain<system_list>>().capacity();
        }

        inline void clear() noexcept
        {
            for_each_in_tuple(m_components, [] (auto & vector)
            {
                vector.clear();
            });
        }

        inline void erase(entity entity)
        {
            for_each_in_tuple(m_components, [entity] (auto & vector)
            {
                vector.erase(vector.begin() + entity);
            });
        }

        inline void erase(entity first, entity last)
        {
            for_each_in_tuple(m_components, [first, last] (auto & vector)
            {
                vector.erase(vector.begin() + first, vector.begin() + last);
            });
        }

        inline size_t max_size() const noexcept
        {
            return get_component<ecs::keychain<system_list>>().max_size();
        }

        inline void reserve(size_t new_cap)
        {
            for_each_in_tuple(m_components, [new_cap] (auto & vector)
            {
                vector.reserve(new_cap);
            });
        }

        inline void resize(size_t count)
        {
            for_each_in_tuple(m_components, [count] (auto & vector)
            {
                vector.resize(count);
            });
        }

        inline void shrink_to_fit()
        {
            for_each_in_tuple(m_components, [] (auto & vector)
            {
                vector.shrink_to_fit();
            });
        }

        inline size_t size() const noexcept
        {
            return get_component<ecs::keychain<system_list>>().size();
        }

    private:
        inline void push_all()
        {
            for_each_in_tuple(m_components, [] (auto & vector)
            {
                using T = std::decay_t<decltype(vector)>;
                vector.push_back(T::value_type());
            });
        }
    };
}
