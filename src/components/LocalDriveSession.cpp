#include "components/LocalDriveSession.hpp"
#include <fstream>
LocalDriveSession::LocalDriveSession(std::filesystem::path file_path)
	: m_data(rn::Json::object()),
	  m_file_path(std::move(file_path))
{
}

LocalDriveSession::~LocalDriveSession() = default;

bool LocalDriveSession::saveToFile()
{
	beforeSave();
	std::ofstream file(m_file_path.string(), std::ios::binary);
	if (!file.is_open())
		return false;
	LocalDriveSessionEncryption encryption;
	std::string json_str = json().dump(4, ' ', true);
	file << encryption.encrypt(json_str, encryption_key);
	file.close();
	return true;
}

bool LocalDriveSession::loadFromFile()
{
	std::ifstream file(m_file_path.string(), std::ios::binary);
	if (!file.is_open())
		return false;
	std::string json_string;
	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);
		json_string += line;
	}
	try
	{
		LocalDriveSessionEncryption encryption;
		m_data = rn::Json::parse(encryption.decrypt(json_string, encryption_key));
	}
	catch (rn::Json::parse_error &error)
	{
		std::cerr << error.what() << "\n";
		throw error;
	}
	afterLoad();
	return true;
}
std::string hash(size_t size, size_t seed)
{
	std::string hash;
	for (size_t i = 0; i < size; i++)
		hash += static_cast<char>(i);
	std::random_device rd;
	std::mt19937_64 g(rd());
	g.seed(seed);
	std::ranges::shuffle(hash, g);
	return hash;
}
std::string LocalDriveSessionEncryption::encrypt(const std::string &value, const size_t key) const
{
	std::string encrypted;
	std::string hash_str = hash(pow(2ull, 8ull * sizeof(std::string::value_type)), key);
	for (auto &c: value)
		encrypted += hash_str[static_cast<int>(c)];
	return encrypted;
}

std::string LocalDriveSessionEncryption::decrypt(const std::string &value, const size_t key) const
{
	std::string decrypted;
	std::string hash_str = hash(pow(2ull, 8ull * sizeof(std::string::value_type)), key);
	for (auto &c: value)
		decrypted += static_cast<char>(hash_str.find(c));
	return decrypted;
}

const rn::Json &LocalDriveSession::json() const
{
	return m_data;
}
