#pragma once

#include "coop/TransferableAction.hpp"

class LevelCompleteAction : public TransferActionBase<LevelCompleteAction>
{
public:
	LevelCompleteAction(const ActionProps &props)
		: TransferActionBase(props)
	{
		
	}
	void play() override
	{
		
	}
};
