#include "coop/Server.hpp"

void Server::start()
{
	type_port();
	int connect_code = bind(getPort(), getIPv4());
	if (connect_code != Done)
	{
		std::cout << "Failed to connect with code: " << connect_code << "\n";
		throw std::exception();
	}
	type_remote_address();
}

void Server::update()
{
	wait_for_message();
	type_and_send_message();
}
