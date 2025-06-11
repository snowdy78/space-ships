#include "game/actions/MoveShipAction.hpp"

#include "Helpers.hpp"
#include "game/AbstractShip.hpp"

MoveShipAction::MoveShipAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (props.authorExist())
		m_ship = *props.castAuthor<AbstractShip>();
}

void MoveShipAction::play()
{
	if (m_ship.expired())
		return;

	auto ship = m_ship.lock();
	ship->move(ship->countMove());
}

rn::Json MoveShipAction::toJson() const
{
	if (m_ship.expired())
		return nullptr;
	auto ship = m_ship.lock();
	return {
		{ md, to_json(ship->getDirection()) }
	};
}

