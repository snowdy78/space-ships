#include "game/ships/ConnectedPlayerShip.hpp"

#include "game/SpaceField.hpp"

ConnectedPlayerShip::ConnectedPlayerShip()
	: AbstractShip(*texture)
{
}

void ConnectedPlayerShip::movement()
{
	AbstractShip::movement();
}

void ConnectedPlayerShip::rotation()
{
	AbstractShip::rotation();
}
void ConnectedPlayerShip::receiveJson(const rn::Json &json)
{
	TransferObjectBase::receiveJson(json);
	std::cout << "conversion is success!\n";
}
void ConnectedPlayerShip::onEvent(sf::Event &event)
{
	AbstractShip::onEvent(event);
}

rn::Vec2f ConnectedPlayerShip::countMove() const
{
	return getAcceleration() * getVelocity() * getMoveDirection();
}

void ConnectedPlayerShip::summonCopy(SpaceField &field) const
{
	field.summonShip<ConnectedPlayerShip>();
}
