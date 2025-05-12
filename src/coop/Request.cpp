#include "coop/Request.hpp"

void Request::receiveJson(const rn::Json &json) 
{
    clear();
    rn::Json::operator=(json);
}

Request::TransferJson Request::toJson() const 
{
	return { id(), *this };
}
