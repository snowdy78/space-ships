#pragma once

#include "coop/TransferableAction.hpp"
#include "game/SpaceFieldObject.hpp"

class DestroySpaceFieldObjectAction : public TransferActionBase<DestroySpaceFieldObjectAction>
{
public:
	DestroySpaceFieldObjectAction(const TransferableActionProps &props = {});

	void play() override;

private:
	SpaceFieldObject *m_what_destroy;
};
