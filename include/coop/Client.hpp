#pragma once
#include "ChatSocket.hpp"

struct Client : ChatSocket
{
	using ChatSocket::ChatSocket;
	void start() override;

	void update() override;
};
