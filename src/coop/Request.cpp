#include "coop/Request.hpp"

const size_t Request::id = TransferableObjectFabric::instance().push<Request>();


void Request::receiveJson(const rn::Json &json) 
{
    clear();
    rn::Json::operator=(json);
}

Request::TransferJson Request::toJson() const 
{
    return {
        id, *this
    };
}
