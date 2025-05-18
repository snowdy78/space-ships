#pragma once

#include <filesystem>
#include "Session.hpp"

class LocalDriveSession : public Session
{
	rn::Json m_data;
	std::filesystem::path m_file_path;

public:
	LocalDriveSession(std::filesystem::path file_path);
	~LocalDriveSession() override;
	bool saveToFile();
	bool loadFromFile();
	virtual void beforeSave()
	{
	}
	virtual void afterLoad()
	{
	}
	template<class KeyT, class ValT>
	rn::Json &at(const KeyT &key) noexcept;
	template<class KeyT>
	const rn::Json &at(const KeyT &key) const noexcept;
	template<class KeyT>
	void erase(const KeyT &key) noexcept;
	const rn::Json &toJson() const override;

	template<class KeyT>
	rn::Json &operator[](const KeyT &key) noexcept;

	template<class KeyT>
	const rn::Json &operator[](const KeyT &key) const noexcept;
};

template<class KeyT, class ValT>
rn::Json &LocalDriveSession::at(const KeyT &key) noexcept
{
	return m_data[key];
}

template<class KeyT>
const rn::Json &LocalDriveSession::at(const KeyT &key) const noexcept
{
	return m_data[key];
}

template<class KeyT>
void LocalDriveSession::erase(const KeyT &key) noexcept
{
	m_data.erase(key);
}

template<class KeyT>
rn::Json &LocalDriveSession::operator[](const KeyT &key) noexcept
{
	return m_data[key];
}

template<class KeyT>
const rn::Json &LocalDriveSession::operator[](const KeyT &key) const noexcept
{
	return m_data[key];
}
