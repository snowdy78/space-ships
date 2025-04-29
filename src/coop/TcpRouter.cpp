#include "coop/TcpRouter.hpp"

TcpRouter::TcpRouter(sf::IpAddress remote_ip, uint16_t remote_port)
	: TcpSocket()
{
	setBlocking(false);
	listener.accept(*this);
}

void TcpRouter::setBlocking(bool blocking)
{
	listener.setBlocking(blocking);
	sf::TcpSocket::setBlocking(false);
}

sf::Socket::Status TcpRouter::connect()
{
	return sf::TcpSocket::connect(getRemoteAddress(), getRemotePort());
}

std::string TcpRouter::encrypt(const std::string &str) 
{
	return str;
}

std::string TcpRouter::decrypt(const std::string &str) 
{
	return str;
}

sf::Socket::Status TcpRouter::sendJson(const rn::Json &send_data) 
{
	std::string json_string = encrypt(send_data.dump(-1, 0));
	sf::Packet packet;
	packet.append(json_string.c_str(), (json_string.size() + 1) * sizeof(char));
	Status status = TcpRouter::send(packet);
	return status;
}

Router::StatusOrJson TcpRouter::receiveJson()
{
	sf::Packet packet;
	Status status	  = listener.listen(getRemotePort(), getRemoteAddress());
	Status recieve_status = receive(packet);
	if (status != sf::Socket::Done && recieve_status != sf::Socket::Done)
		return status;
	std::string str{ static_cast<const char *>(packet.getData()) };
	rn::Json json = rn::Json::parse(decrypt(str));
	return json;
}
