#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

TransferableAction::TransferableAction(GameObject *author, GameObject *contributor, const rn::Json &props)
    : author(author ? std::optional<size_t>{author->getGameObjectId()} : std::nullopt),
    contributor(contributor ? std::optional<size_t>{contributor->getGameObjectId()} : std::nullopt)
{
    
}

