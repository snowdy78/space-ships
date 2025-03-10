#pragma once

#include "game/actions/AbstractAction.hpp"
#include "Transferable.hpp"

class TransferableAction : public AbstractAction, public Transferable
{
public:
    TransferableAction() = delete;
    TransferableAction(std::optional<GameObject *> author_id, std::optional<GameObject *> target_id)
        : Transferable(Transferable::action)
    {
        
    }
};
