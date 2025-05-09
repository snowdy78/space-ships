#include "game/actions/MoveShipAction.hpp"
#include <stdexcept>
#include "game/AbstractShip.hpp"

const size_t MoveShipAction::id = identify<MoveShipAction>();

MoveShipAction::MoveShipAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: TransferableAction(author, contributor, props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(author))
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
	return { id, { { md, to_json(m_ship ? m_ship->getDirection() : rn::Vec2f{}) } } };
}

AbstractAction *MoveShipAction::copy() const
{
	return new MoveShipAction(m_ship, nullptr);
}
