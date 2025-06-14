#include "components/GameConfiguration.hpp"

bool GameConfiguration::load()
{
	rn::JsonFile file;
	try
	{
		file.load("src/config.json");
	}
	catch (std::exception &)
	{
		return false;
	}
	instance().m_data = std::move(file.json);
	return true;
}

GameConfiguration::ItemPointer::ItemPointer(std::string &&string)
	: m_key(std::move(string))
{
}

GameConfiguration::ItemPointer::ItemPointer(const ItemPointer *config, std::string &&string)
	: m_before(config),
	  m_key(std::move(string))
{
}

void GameConfiguration::ItemPointer::initJson() const
{
	static auto loaded = load();
	if (!loaded)
		throw std::runtime_error("game configuration was not load!");
	const rn::Json *json_ptr = m_before ? &**m_before : &instance().m_data;
	try
	{
		if (!json_ptr->contains(m_key))
		{
			if (!m_default)
				auto exc = json_ptr->at(m_key);
			json->reset(new rn::Json(*m_default));
			return;
		}
		auto i = json_ptr->at(m_key);
		json->reset(new rn::Json(std::move(i)));
	}
	catch (rn::Json::out_of_range &error)
	{
		std::cerr << error.what() << "\n";
		std::cerr << "game configuration error occurs! (maybe unknown key: '" << m_key << "')"
				  << (m_before ? " from '" + m_before->m_key + "'" : "") << "\n";
		throw;
	}
}

rn::Json &GameConfiguration::ItemPointer::operator*()
{
	if (!*json)
		initJson();
	return **json;
}

const rn::Json &GameConfiguration::ItemPointer::operator*() const
{
	if (!*json)
		initJson();
	return **json;
}

rn::Json *GameConfiguration::ItemPointer::operator->()
{
	if (!*json)
		initJson();
	return json->get();
}

const rn::Json *GameConfiguration::ItemPointer::operator->() const
{
	if (!*json)
		initJson();
	return json->get();
}

GameConfiguration::ItemPointer &GameConfiguration::ItemPointer::get(std::string &&key) const
{
	return instance().get(*this, std::move(key));
}

GameConfiguration::ItemPointer &GameConfiguration::ItemPointer::set_default(rn::Json &&default_value)
{
	*m_default = rn::Json(std::move(default_value));
	return *this;
}

GameConfiguration &GameConfiguration::instance()
{
	static GameConfiguration config;
	return config;
}

void GameConfiguration::preload()
{
	for (auto &item: instance().m_config_items)
		**item;
}

GameConfiguration::ItemPointer &GameConfiguration::get(std::string &&key)
{
	m_config_items.emplace_back(new ItemPointer(std::move(key)));
	return *m_config_items.back();
}

GameConfiguration::ItemPointer &GameConfiguration::get(const ItemPointer &config_item, std::string &&key)
{
	m_config_items.emplace_back(new ItemPointer(&config_item, std::move(key)));
	return *m_config_items.back();
}
