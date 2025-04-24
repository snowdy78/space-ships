#include "coop/Request.hpp"

const size_t Request::id = TransferableObjectFabric::instance().push<Request>();

Request::Request(const rn::Json &data)
    : object(id, data)
{

}

rn::Json &Request::at(const char *key) 
{
    return object.data().at(key);
}

const rn::Json &Request::at(const char *key) const 
{
    return object.data().at(key);
}

Request::TransferJson Request::toJson() const 
{
    return object;
}
