#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

TransferableActionProps::TransferableActionProps(GameObject *author, GameObject *contributor, const rn::Json &props): author(author),
	contributor(contributor),
	props(props)
{
		
}

TransferableAction::TransferableAction(GameObject *author, GameObject *contributor, const rn::Json &props)
    : author(author ? std::optional<size_t>{author->getGameObjectId()} : std::nullopt),
    contributor(contributor ? std::optional<size_t>{contributor->getGameObjectId()} : std::nullopt)
{
    
}

