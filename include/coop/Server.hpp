#pragma once
#include "ChatSocket.hpp"

struct Server : ChatSocket
{
	using ChatSocket::ChatSocket;
	void start() override;

	void update() override;
};
