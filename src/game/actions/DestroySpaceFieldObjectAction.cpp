#include "game/actions/DestroySpaceFieldObjectAction.hpp"

DestroySpaceFieldObjectAction::DestroySpaceFieldObjectAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (auto obj = dynamic_cast<SpaceFieldObject *>(props.author))
	{
		m_what_destroy = obj;
	}
}

void DestroySpaceFieldObjectAction::play()
{
	if (!m_what_destroy)
		return;

	m_what_destroy->destroy();
}

