#include "game/actions/ShipShootAction.hpp"

const size_t ShipShootAction::id = identify<ShipShootAction>();

ShipShootAction::ShipShootAction(std::optional<GameObject *> author, std::optional<GameObject *> target, const rn::Json &props)
	: TransferableAction(author, target, props)
{}

void ShipShootAction::play() 
{
	
}

TransferableAction::TransferJson ShipShootAction::toJson() const 
{
	return {
		id
	};
}

AbstractAction *ShipShootAction::copy() const 
{
	return new ShipShootAction();
}
