#include "coop/TransferableObject.hpp"

TransferableObjectFabric &TransferableObjectFabric::instance()
{
	static TransferableObjectFabric instance;
	return instance;
}

const TransferableObjectFabric::create_func &TransferableObjectFabric::get(size_t id) 
{
    return transfer_objects.at(id);
}


void TransferableObjectFabric::erase(size_t id) {}

void TransferableObjectFabric::clear() {}
