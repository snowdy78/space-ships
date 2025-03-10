#include "coop/TransferableActionFabric.hpp"

TransferableActionFabric &TransferableActionFabric::instance()
{
    static TransferableActionFabric t;
    return t;
}

const TransferableActionFabric::create_action_func &TransferableActionFabric::get(size_t id) 
{
    return transfer_actions.at(id);
}

void TransferableActionFabric::erase(size_t id) 
{
    transfer_actions.erase(id);
}

void TransferableActionFabric::clear() 
{
    transfer_actions.clear();
}
