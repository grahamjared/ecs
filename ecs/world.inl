#pragma once

#include "world.h"

namespace ecs
{
	template <class system_list, class ...Ts>
	template <class T>
	inline auto & world<system_list, Ts...>::get_component()
	{
		return std::get<std::vector<T>>(m_components);
	}

	template<class system_list, class ...Ts>
	template<class T>
	inline const auto & world<system_list, Ts...>::get_component() const
	{
		return std::get<std::vector<T>>(m_components);
	}

	template<class system_list, class ...Ts>
	template<class T>
	inline auto & world<system_list, Ts...>::get(ecs::entity entity)
	{
		return std::get<std::vector<T>>(m_components)[entity];
	}

	template<class system_list, class ...Ts>
	template<class T>
	inline const auto & world<system_list, Ts...>::get(ecs::entity entity) const
	{
		return std::get<std::vector<T>>(m_components)[entity];
	}

	template<class system_list, class ...Ts>
	inline auto & world<system_list, Ts...>::keychain(ecs::entity entity)
	{
		return std::get<std::vector<ecs::keychain<system_list>>>(m_components)[entity];
	}

	template<class system_list, class ...Ts>
	inline const auto & world<system_list, Ts...>::keychain(ecs::entity entity) const
	{
		return std::get<std::vector<ecs::keychain<system_list>>>(m_components)[entity];
	}

	template<class system_list, class ...Ts>
	inline entity world<system_list, Ts...>::create()
	{
		for (entity entity = 0; entity < size(); ++entity)
			if (get<ecs::keychain<system_list>>(entity).empty())
				return entity;

		push_all();
		return size() - 1;
	}

	template<class system_list, class ...Ts>
	inline void world<system_list, Ts...>::destroy(entity entity) noexcept
	{
		get<ecs::keychain<system_list>>(entity).clear();
	}

	template<class system_list, class ...Ts>
	inline bool world<system_list, Ts...>::valid(entity entity) const noexcept
	{
		return (entity > -1 && entity < static_cast<int>(size()));
	}

	template<class system_list, class ...Ts>
	inline bool world<system_list, Ts...>::exists(entity entity) const noexcept
	{
		if (!valid(entity))
			return false;
		else
			return !get<ecs::keychain<system_list>>(entity).empty();
	}

	template<class system_list, class ...Ts>
	inline size_t world<system_list, Ts...>::capacity() noexcept
	{
		return get_component<ecs::keychain<system_list>>().capacity();
	}

	template<class system_list, class ...Ts>
	inline void world<system_list, Ts...>::clear() noexcept
	{
		for_each_in_tuple(m_components, [] (auto & vector)
		{
			vector.clear();
		});
	}

	template<class system_list, class ...Ts>
	inline void world<system_list, Ts...>::erase(entity entity)
	{
		for_each_in_tuple(m_components, [entity] (auto & vector)
		{
			vector.erase(vector.begin() + entity);
		});
	}

	template<class system_list, class ...Ts>
	inline void world<system_list, Ts...>::erase(entity first, entity last)
	{
		for_each_in_tuple(m_components, [first, last] (auto & vector)
		{
			vector.erase(vector.begin() + first, vector.begin() + last);
		});
	}

	template<class system_list, class ...Ts>
	inline size_t world<system_list, Ts...>::max_size() const noexcept
	{
		return get_component<ecs::keychain<system_list>>().max_size();
	}

	template<class system_list, class ...Ts>
	inline void world<system_list, Ts...>::reserve(size_t new_cap)
	{
		for_each_in_tuple(m_components, [new_cap] (auto & vector)
		{
			vector.reserve(new_cap);
		});
	}

	template<class system_list, class ...Ts>
	inline void world<system_list, Ts...>::resize(size_t count)
	{
		for_each_in_tuple(m_components, [count] (auto & vector)
		{
			vector.resize(count);
		});
	}

	template<class system_list, class ...Ts>
	inline void world<system_list, Ts...>::shrink_to_fit()
	{
		for_each_in_tuple(m_components, [] (auto & vector)
		{
			vector.shrink_to_fit();
		});
	}

	template<class system_list, class ...Ts>
	inline size_t world<system_list, Ts...>::size() const noexcept
	{
		return get_component<ecs::keychain<system_list>>().size();
	}

	template<class system_list, class ...Ts>
	inline entity world<system_list, Ts...>::copy(ecs::entity entity)
	{
		auto copy = this->create();
		for_each_in_tuple(m_components, [copy, entity] (auto & vector)
		{
			vector[copy] = vector[entity];
		});

		return copy;
	}

	template<class system_list, class ...Ts>
	inline void world<system_list, Ts...>::push_all()
	{
		for_each_in_tuple(m_components, [] (auto & vector)
		{
			using T = std::decay_t<decltype(vector)>;
			vector.push_back(T::value_type());
		});
	}
}
