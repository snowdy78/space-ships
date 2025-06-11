#pragma once

#include "decl.hpp"

class GameConfiguration
{
	GameConfiguration() = default;

public:
	static GameConfiguration &instance()
	{
		static GameConfiguration config;
		return config;
	}
	static bool load()
	{
		rn::JsonFile file;
		if (!file.load("src/config.json"))
			return false;
		instance().m_data = file.json;
		return true;
	}
	auto get(const std::string &key)
	{
#ifdef SPACE_SHIP_DEBUG
		try
		{
#endif
			return m_data.at(key);
#ifdef SPACE_SHIP_DEBUG
		}
		catch (std::out_of_range &error)
		{
			std::cerr << error.what() << "\n";
			std::cerr << "game configuration error occurs! (unknown key: '" << key << "')\n";
			throw error;
		}
#endif
	}

private:
	rn::Json m_data;
};

using config = GameConfiguration;
