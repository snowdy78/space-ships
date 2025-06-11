#include "game/actions/DestroySpaceFieldObjectAction.hpp"

DestroySpaceFieldObjectAction::DestroySpaceFieldObjectAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (props.authorExist())
	{
		m_what_destroy = *props.castAuthor<SpaceFieldObject>();
	}
}

void DestroySpaceFieldObjectAction::play()
{
	if (m_what_destroy.expired())
		return;
	if (auto destroyable = m_what_destroy.lock(); !destroyable->willBeDestroyed())
	{
		destroyable->destroy();
	}
}

