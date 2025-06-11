#include "game/actions/AccelerateShipAction.hpp"
#include "game/AbstractShip.hpp"

AccelerateShipAction::AccelerateShipAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (props.authorExist())
	{
		m_ship_ptr = *props.castAuthor<AbstractShip>();
	}
	if (!(props.props.contains(acceleration) && props.props[acceleration].is_number()))
		throw std::runtime_error("Failed to accelerate ship, props is not correct");
	m_acceleration = props.props[acceleration];
}

void AccelerateShipAction::play()
{
	if (m_ship_ptr.expired())
		return;
	auto ship = m_ship_ptr.lock();
	ship->setAcceleration(m_acceleration);
}

rn::Json AccelerateShipAction::toJson() const
{
	return {
		{ acceleration, m_acceleration }
	};
}
