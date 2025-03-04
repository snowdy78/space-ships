#include "coop/ChatSocket.hpp"
#include "coop/Client.hpp"
#include "coop/Server.hpp"

ChatSocket::ChatSocket()
{
	setBlocking(true);
}

ChatSocket::~ChatSocket()
{
	delete port;
	delete remote_port;
}

void ChatSocket::type_remote_address()
{
	std::cout << "type remote address 'ip:port': ";
	std::string address;
	std::cin >> address;
	std::regex ip_port_rgx(R"(([0-9]*\.[0-9]*\.[0-9]*\.[0-9]*|local|l):([0-9]{5}))");
	std::smatch matches;
	if (std::regex_search(address, matches, ip_port_rgx))
	{
		if (matches.size() == 3)
		{
			remote_ip_address = std::regex_match(address, std::regex("l|local")) ? sf::IpAddress::getLocalAddress()
																				 : sf::IpAddress(matches[1]);
			remote_port		  = new unsigned short(std::stoi(matches[2]));
		}
	}
}

void ChatSocket::type_port()
{
	unsigned short port;
	std::cout << "type port: ";
	std::cin >> port;
	this->port = new unsigned short(port);
	std::cout << "my address is: " << ip_address << ":" << port << "\n";
}

void ChatSocket::assign_port(unsigned short port)
{
	this->port = new unsigned short(port);
	std::cout << "my address is: " << ip_address << ":" << port << "\n";
}

bool ChatSocket::portIsOpen() const
{
	return port != nullptr;
}

unsigned short ChatSocket::getPort() const
{
	return *port;
}

unsigned short ChatSocket::getRemotePort() const
{
	return *remote_port;
}

sf::IpAddress ChatSocket::getIPv4() const
{
	return ip_address;
}

sf::IpAddress ChatSocket::getRemoteIPv4() const
{
	return remote_ip_address;
}

void ChatSocket::type_and_send_message()
{
	if (!port)
	{
		std::cout << "port is unknown\n";
		return;
	}
	std::string str;
	std::cout << "message: ";
	std::getline(std::cin, str);
	packet.clear();
	packet.append(str.c_str(), str.size() * sizeof(char));
	Status send_code = send(packet, remote_ip_address, *remote_port);

	if (send_code != Done)
	{
		std::cout << "Failed to send with code" << send_code << "\n";
	}
}

void ChatSocket::wait_for_message()
{
	if (!port)
	{
		std::cout << "port is unknown\n";
		return;
	}
	std::cout << "wait for message\n";

	Status status;
	do
	{
		packet.clear();
		status = receive(packet, remote_ip_address, *remote_port);
	}
	while (status != Done && status != Partial);
	size_t size = packet.getDataSize();
	char *p		= new char[size + 1];
	std::memcpy(p, packet.getData(), size);
	*(p + size) = '\0';
	std::cout << p << "\n";
	packet.clear();
	delete[] p;
}


ChatSocket *getSocketType()
{
	std::string c;

	while (true)
	{
		std::cout << "host or connect? (h/c): ";

		std::cin >> c;
		const std::regex client_socket{ "c|connect", std::regex_constants::icase };
		const std::regex server_socket{ "h|host", std::regex_constants::icase };
		if (std::regex_match(c.begin(), c.end(), client_socket))
		{
			return new Client;
		}
		if (std::regex_match(c.begin(), c.end(), server_socket))
		{
			return new Server;
		}
		std::cout << "incorrect.\n";
	}
	return nullptr;
}

void startChatting()
{
	ChatSocket *socket = getSocketType();
	sf::Packet packet;

	try
	{
		socket->start();

		while (true)
		{
			socket->update();
		}
	}
	catch (std::exception &err)
	{
		std::cin.get();
		delete socket;
		throw err;
	}

	std::cin.get();
	delete socket;
}
