#include "coop/Request.hpp"

void Request::receiveJson(const rn::Json &json) 
{
    clear();
    rn::Json::operator=(json);
}

rn::Json Request::getData() const 
{
	return static_cast<const rn::Json &>(*this);
}
