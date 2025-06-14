#pragma once

#include <filesystem>
#include <list>
#include <string>
#include "decl.hpp"
#ifndef G_CONFIG_PROP_DEFINE
#define G_CONFIG_PROP_DEFINE(current_instance, prop_name) \
	inline static auto &prop_name = (current_instance).get(#prop_name)
#endif

class GameConfiguration
{
	GameConfiguration() = default;
	static bool load();

public:
	template<class T>
	using container_type = std::list<T>;
	class ItemPointer
	{
		template<class T>
		friend class std::list;
		friend class GameConfiguration;

		explicit ItemPointer(std::string &&string);
		explicit ItemPointer(const ItemPointer *config, std::string &&string);
		void initJson() const;

	public:
		rn::Json &operator*();
		const rn::Json &operator*() const;
		rn::Json *operator->();
		const rn::Json *operator->() const;
		ItemPointer &get(std::string &&key) const;
		ItemPointer &set_default(rn::Json &&default_value);

	private:
		template<class T>
		using pointer_type = std::unique_ptr<T>;
		using string_type = std::string;
		using container_type = std::list<string_type>;

		const ItemPointer *m_before = nullptr;
		std::unique_ptr<rn::Json> m_default{nullptr}; 
		string_type m_key;
		pointer_type<pointer_type<rn::Json>> json = std::make_unique<pointer_type<rn::Json>>(nullptr);
	};
	
	using value_type = std::unique_ptr<ItemPointer>;
	using items_type = container_type<value_type>;
	using item = ItemPointer;
	static GameConfiguration &instance();
	static void preload();

	ItemPointer &get(std::string &&key);
	ItemPointer &get(const ItemPointer &config_item, std::string &&key);
private:
	constexpr static const char *config_path = "./config.json";
	rn::Json m_data;
	items_type m_config_items;
};

using config		  = GameConfiguration;
