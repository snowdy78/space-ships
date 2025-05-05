#include "game/actions/AccelerateShipAction.hpp"
#include "game/AbstractShip.hpp"

const size_t AccelerateShipAction::id = identify<AccelerateShipAction>();

AccelerateShipAction::AccelerateShipAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: TransferableAction(author, contributor, props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(author))
		m_ship = ship;
	if (!(props.contains(acceleration) && props[acceleration].is_number()))
		throw std::runtime_error("Failed to accelerate ship, props is not correct");
	m_acceleration = props[acceleration];
}

void AccelerateShipAction::play()
{
	if (!m_ship)
		return;
    m_ship->setAcceleration(m_acceleration);
}

Transferable::TransferJson AccelerateShipAction::toJson() const
{
	return { id, { { acceleration, m_acceleration } } };
}

AbstractAction *AccelerateShipAction::copy() const
{
	return new AccelerateShipAction(
		m_ship, nullptr,
		{
			{ acceleration, m_acceleration }
	 }
	);
}
