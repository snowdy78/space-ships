#pragma once

#include <filesystem>
#include "Session.hpp"
#include "EncryptionBase.hpp"

class LocalDriveSessionEncryption : public EncryptionBase<std::string>
{
public:
	std::string encrypt(const std::string &value, size_t key) const override;
	std::string decrypt(const std::string &value, size_t key) const override;
};

class LocalDriveSession : public Session
{
	constexpr static size_t encryption_key = 200;

protected:
	virtual void beforeSave()
	{
	}
	virtual void afterLoad()
	{
	}
public:
	LocalDriveSession(std::filesystem::path file_path);
	~LocalDriveSession() override;
	bool saveToFile();
	bool loadFromFile();
	
	template<class KeyT, class ValT>
	rn::Json &at(const KeyT &key) noexcept;
	template<class KeyT>
	const rn::Json &at(const KeyT &key) const noexcept;
	template<class KeyT>
	void erase(const KeyT &key) noexcept;
	const rn::Json &json() const override;

	template<class KeyT>
	rn::Json &operator[](const KeyT &key) noexcept;

	template<class KeyT>
	const rn::Json &operator[](const KeyT &key) const noexcept;
private:
	rn::Json m_data{  };
	std::filesystem::path m_file_path;
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
