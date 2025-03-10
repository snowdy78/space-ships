#include "game/actions/ShipShootAction.hpp"
#include "coop/TransferableAction.hpp"

ShipShootAction::ShipShootAction()
	: TransferableAction()
{}

void ShipShootAction::play() 
{
	
}

rn::Json ShipShootAction::toJson() const 
{
	return {
		{ "id", id }
	};
}

AbstractAction *ShipShootAction::copy() const 
{
	return new ShipShootAction();
}
