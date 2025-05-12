#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

TransferableActionProps::TransferableActionProps(GameObject *author, GameObject *contributor, const rn::Json &props)
	: author(author),
	  contributor(contributor),
	  data(props)
{
}

TransferableAction::TransferableAction(const TransferableActionProps &props)
	: author(props.author ? std::optional{ props.author->getGameObjectId() } : std::nullopt),
	  contributor(props.contributor ? std::optional{ props.contributor->getGameObjectId() } : std::nullopt)
{
}
