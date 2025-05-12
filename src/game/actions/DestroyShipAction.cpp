#include "game/actions/DestroyShipAction.hpp"
#include "game/AbstractShip.hpp"
#include "game/DamageDealer.hpp"
#include "game/GameGlobals.hpp"

DestroyShipAction::DestroyShipAction(const TransferableActionProps &props)
	: BaseTransferableAction(props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(props.author))
		m_ship = ship;
	try
	{
		if (auto destroyer = dynamic_cast<DamageDealer *>(props.contributor))
			m_destroyer = destroyer;
	}
	catch (std::bad_cast &err)
	{
	}
}

void DestroyShipAction::play()
{
	if (!m_ship)
		return;
	if (GameGlobals::exist())
		GameGlobals::instance().field.remove(m_ship);
}

DestroyShipAction::TransferJson DestroyShipAction::toJson() const
{
	return { id() };
}

AbstractAction *DestroyShipAction::copy() const
{
	return new DestroyShipAction({ m_ship, m_destroyer });
}
