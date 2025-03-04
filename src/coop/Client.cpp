#include "coop/Client.hpp"

void Client::start()
{
	type_port();
	type_remote_address();
}

void Client::update()
{
	type_and_send_message();
	wait_for_message();
}
