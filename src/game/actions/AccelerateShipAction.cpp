#include "game/actions/AccelerateShipAction.hpp"
#include "game/AbstractShip.hpp"

AccelerateShipAction::AccelerateShipAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(props.author))
		m_ship = ship;
	if (!(props.props.contains(acceleration) && props.props[acceleration].is_number()))
		throw std::runtime_error("Failed to accelerate ship, props is not correct");
	m_acceleration = props.props[acceleration];
}

void AccelerateShipAction::play()
{
	if (!m_ship)
		return;
	m_ship->setAcceleration(m_acceleration);
}

rn::Json AccelerateShipAction::toJson() const
{
	return {
		{ acceleration, m_acceleration }
	};
}
