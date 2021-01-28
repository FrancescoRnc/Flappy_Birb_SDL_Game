#pragma once
#include <vector>
#include <bitset>
#include <tuple>
#include <utility>

template<typename... Components>
class ecs
{
	template<size_t Index, typename Type, typename First, typename... Other>
	constexpr auto index()
	{
		return std::is_same_v<Type, First> ? Index : index<Index + 1, Type, Other...>();
	}

	template<size_t Index, typename Type>
	constexpr auto index()
	{
		return 0;
	}

	public:
	using entity_t = size_t;

	entity_t Create()
	{
		entity_t entity;
		if (pending.empty())
		{
			entity = entities.size();
			entities.push_back({}); // Con questo {} è come specificare il costruttore di default
		}
		else
		{
			auto entity = pending.back();
			pending.pop_back();
		}
		return entity;
	}

	void Destroy(entity_t entity)
	{
		pending.push_back(entity);
		entities[entity].reset();
	}

	template<typename Type, typename... Args>
	Type& Assign(entity_t entity, Args &&... args)
	{
		auto &pool = std::get<std::vector<Type>>(pools);
		if (pool.size() <= entity)
		{
			pool.resize(entity + 1);
		}

		entities[entity].set(index<0, Type, Components...>());
		return (pool[entity] = Type{std::forward<Args>(args)...});
	}

	template<typename Type>
	void Remove(entity_t entity)
	{
		entities[entity].reset(index<0, Type, Components...>());
	}

	template<typename Type>
	bool Has(entity_t entity)
	{
		return entities[entity].test(index<0, Type, Components... >());
	}

	template<typename Type>
	Type & Get(entity_t entity)
	{
		auto &pool = std::get<std::vector<Type>>(pools);
		return pool[entity];
	}

	template<typename Type>
	Type * Try_Get(entity_t entity)
	{
		return Has<Type>(entity) ? &Get<Type>(entity) : nullptr;
	}

	template<typename... Type, typename Func>
	void for_each(Func func)
	{
		for (entity_t entity = 0; entity < entities.size(); ++entity)
		{
			if ((entities[entity].test(index<0, Type, Components...>()) && ...))
			{
				func(entity, Get<Type>(entity)...);
			}
		}
	}

	private:
	std::vector<std::bitset<sizeof...(Components)>> entities;
	std::tuple<std::vector<Components>...> pools;
	std::vector<entity_t> pending;
};