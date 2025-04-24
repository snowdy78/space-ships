#include "game/actions/ShipShootAction.hpp"
#include "game/AbstractShip.hpp"

const size_t ShipShootAction::id = identify<ShipShootAction>();

ShipShootAction::ShipShootAction(GameObject *author, GameObject *target, const rn::Json &props)
	: TransferableAction(author, target, props)
{
	auto ship = dynamic_cast<AbstractShip *>(author);
	auto d = "direction", x = "x", y = "y";
	if (!ship || !(props.contains(d) && props[d].is_object() && props[d].contains(x) && props[d].contains(y) && props[d][x].is_number() && props[d][y].is_number()))
	{
		throw std::runtime_error("ShipShootAction failed to initialize");
	}
	this->ship = ship;
	rn::Vec2f direction{ props[d][x].get<float>(), props[d][y].get<float>() };
	this->direction = rn::math::norm(direction);
}

void ShipShootAction::play() 
{
	if (ship)
	{
		ship->setDirection(direction);
		ship->shoot();
	}
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
