#include "game/actions/MoveShipAction.hpp"
#include <stdexcept>
#include "game/AbstractShip.hpp"

const size_t MoveShipAction::id = identify<MoveShipAction>();

MoveShipAction::MoveShipAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: TransferableAction(author, contributor, props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(author))
		m_ship = ship;

	if (!(props.contains(md) && props[md].contains("x") && props[md]["x"].is_number() && props[md].contains("y")
		  && props[md]["y"].is_number()))
	{
		throw std::runtime_error("Failed to move a ship, props value is not correct");
	}
	m_move_direction = { props[md]["x"], props[md]["y"] };
}

void MoveShipAction::play()
{
	if (!m_ship)
		return;
	rn::Vec2f offset = m_ship->getAcceleration() * m_ship->getVelocity() * (m_move_direction);
	m_ship->move(offset);
}

Transferable::TransferJson MoveShipAction::toJson() const
{
	return { id, { { md, to_json(m_move_direction) } } };
}

AbstractAction *MoveShipAction::copy() const
{
	return new MoveShipAction(
		m_ship, nullptr,
		{
			{ md, to_json(m_move_direction) }
	  }
	);
}
