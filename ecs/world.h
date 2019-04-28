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
		using keychain_t = ecs::keychain<system_list>;
		using vectors_t = std::tuple<std::vector<keychain_t>, std::vector<Ts>... >;

        vectors_t m_components;

	public:
		struct iterator;
		struct entity_view
		{
			friend class iterator;

			inline entity_view(world<system_list, Ts...> & world, ecs::entity position)
				: m_world(world)
				, m_entity(position)
			{}

			template <class T>
			inline auto & get()
			{
				return m_world.get<T>(m_entity);
			}

			template <class T>
			inline const auto & get() const
			{
				return m_world.get<T>(m_entity);
			}

		private:
			world<system_list, Ts...> & m_world;
			ecs::entity m_entity = 0;
		};

		struct iterator
		{
			using value_type = entity_view;

			inline iterator(world<system_list, Ts...> & world, ecs::entity entity)
				: m_view(world, entity)
			{}

		public:
			inline entity_view & operator*() { return m_view; }
			inline bool          operator!=(const iterator & other) const
			{
				if (&m_view.m_world == &other.m_view.m_world)
					return m_view.m_entity != other.m_view.m_entity;
				else
					return false;
			}
			inline iterator & operator++()
			{
				m_view.m_entity++;
				return *this;
			}

		private:
			entity_view m_view;
		};

    public:
		inline iterator begin() { return iterator(*this, 0); }
		inline iterator end()	{ return iterator(*this, size()); }

    public:
        ////////////////////////////////////////////////////////////
        // Retrieves a vector<Ts> & (a vector of components) at the
        // index <I>
        ////////////////////////////////////////////////////////////
        template <class T>
		inline auto & get_component();

        template <class T>
		inline const auto & get_component() const;

        ////////////////////////////////////////////////////////////
        // get an entity's component based on type
        ////////////////////////////////////////////////////////////
        template <class T>
		inline auto & get(ecs::entity entity);

        template <class T>
		inline const auto & get(ecs::entity entity) const;

		inline auto & keychain(ecs::entity entity);

		inline const auto & keychain(ecs::entity entity) const;

        ////////////////////////////////////////////////////////////
        // First tries to find a deleted entity (an entity with all keys empty) and repurpose that as a new entity
        // If that fails, it pushes back all components and returns the newly allocated entity
        ////////////////////////////////////////////////////////////
		inline entity create();

        ////////////////////////////////////////////////////////////
        // All destroying an entity does is clear its keychain
        // This marks it as destroyed, so it can be repurposed without reallocating more memory
        ////////////////////////////////////////////////////////////
		inline void destroy(entity entity) noexcept;

        ////////////////////////////////////////////////////////////
        // Tests if a entity is within the bounds of the world
        ////////////////////////////////////////////////////////////
		inline bool valid(entity entity) const noexcept;

        ////////////////////////////////////////////////////////////
        // Tests if an entity has at least one non-empty key
        // (aka still being used by a system)
        ////////////////////////////////////////////////////////////
		inline bool exists(entity entity) const noexcept;

        ////////////////////////////////////////////////////////////
        // Below are functions identical to std::vector:: functions
        // They work exactly the same, but operate on all vectors
        ////////////////////////////////////////////////////////////

		inline size_t capacity() noexcept;
		inline void clear() noexcept;
		inline void erase(entity entity);
		inline void erase(entity first, entity last);        
		inline size_t max_size() const noexcept;
		inline void reserve(size_t new_cap);
		inline void resize(size_t count);
		inline void shrink_to_fit();
		inline size_t size() const noexcept;
		inline entity copy(ecs::entity entity);

    private:
		inline void push_all();
    };
}

#include "world.inl"
