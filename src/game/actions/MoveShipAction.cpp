#include "game/actions/MoveShipAction.hpp"
#include <stdexcept>
#include "game/AbstractShip.hpp"

MoveShipAction::MoveShipAction(const TransferableActionProps &props)
	: BaseTransferableAction(props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(props.author))
		m_ship = ship;
}

void MoveShipAction::play()
{
	if (!m_ship)
		return;
	m_ship->move(m_ship->countMove());
}

Transferable::TransferJson MoveShipAction::toJson() const
{
	return { id(), { { md, to_json(m_ship ? m_ship->getDirection() : rn::Vec2f{}) } } };
}

AbstractAction *MoveShipAction::copy() const
{
	return new MoveShipAction({ m_ship, nullptr });
}
