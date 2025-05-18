#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

TransferableActionProps::TransferableActionProps(GameObject *author, GameObject *contributor, rn::Json props)
	: author(author),
	  contributor(contributor),
	  props(std::move(props))
{
}

TransferableAction::TransferableAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: author(author ? std::optional<size_t>{ author->hash() } : std::nullopt),
	  contributor(contributor ? std::optional<size_t>{ contributor->hash() } : std::nullopt)
{
}
