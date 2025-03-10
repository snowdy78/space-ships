#include "coop/TransferableFabric.hpp"

TransferableFabric &TransferableFabric::instance()
{
	static TransferableFabric instance;
	return instance;
}

const TransferableFabric::create_func &TransferableFabric::get(size_t id) 
{
    return transfer_objects.at(id);
}


void TransferableFabric::erase(size_t id) {}

void TransferableFabric::clear() {}
