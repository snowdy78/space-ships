#pragma once

#include "decl.hpp"

struct ChatSocket : sf::UdpSocket, rn::LogicalObject
{
	using sf::UdpSocket::UdpSocket;
	ChatSocket();

	virtual ~ChatSocket() = 0;
	void type_remote_address();

	void type_port();

	void assign_port(unsigned short port);

	bool portIsOpen() const;

	unsigned short getPort() const;

	unsigned short getRemotePort() const;

	sf::IpAddress getIPv4() const;

	sf::IpAddress getRemoteIPv4() const;

	void type_and_send_message();

	void wait_for_message();

protected:
	sf::Packet packet{};

private:
	unsigned short *remote_port		= nullptr;
	unsigned short *port			= nullptr;
	sf::IpAddress ip_address		= sf::IpAddress::getLocalAddress();
	sf::IpAddress remote_ip_address = "";
};

ChatSocket *getSocketType();

void startChatting();
