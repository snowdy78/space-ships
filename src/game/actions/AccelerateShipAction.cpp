#include "game/actions/AccelerateShipAction.hpp"
#include "game/AbstractShip.hpp"


AccelerateShipAction::AccelerateShipAction(const TransferableActionProps &props)
	: BaseTransferableAction(props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(props.author))
		m_ship = ship;
	if (!(props.data.contains(acceleration) && props.data[acceleration].is_number()))
		throw std::runtime_error("Failed to accelerate ship, props is not correct");
	m_acceleration = props.data[acceleration];
}

void AccelerateShipAction::play()
{
	if (!m_ship)
		return;
	m_ship->setAcceleration(m_acceleration);
}

Transferable::TransferJson AccelerateShipAction::toJson() const
{
	return { id(), { { acceleration, m_acceleration } } };
}

AbstractAction *AccelerateShipAction::copy() const
{
	return new AccelerateShipAction({ m_ship, nullptr, { { acceleration, m_acceleration } } });
}
