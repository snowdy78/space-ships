#include "components/LocalDriveSession.hpp"

LocalDriveSession::LocalDriveSession(std::filesystem::path file_path)
	: m_file_path(std::move(file_path))
{
}

LocalDriveSession::~LocalDriveSession() = default;

bool LocalDriveSession::saveToFile()
{
	beforeSave();
	rn::JsonFile file;
	if (!file.load(m_file_path.string()))
	{
		return false;
	}
	file.json = toJson();
	file.save();
	return true;
}

bool LocalDriveSession::loadFromFile()
{
	rn::JsonFile file;
	if (!file.load(m_file_path.string()))
	{
		return false;
	}
	m_data = file.json;
	afterLoad();
	return true;
}

const rn::Json & LocalDriveSession::toJson() const
{
	return m_data;
}
