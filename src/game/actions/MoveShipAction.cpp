#include "game/actions/MoveShipAction.hpp"
#include <stdexcept>
#include "game/AbstractShip.hpp"

MoveShipAction::MoveShipAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(getAuthor()))
		m_ship = ship;
}

void MoveShipAction::play()
{
	if (!m_ship)
		return;
	m_ship->move(m_ship->countMove());
}

rn::Json MoveShipAction::toJson() const
{
	return {
		{ md, to_json(m_ship ? m_ship->getDirection() : rn::Vec2f{}) }
	};
}

