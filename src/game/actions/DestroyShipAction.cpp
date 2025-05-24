#include "game/actions/DestroyShipAction.hpp"
#include "game/AbstractShip.hpp"
#include "game/DamageDealer.hpp"
#include "game/GameManager.hpp"

DestroyShipAction::DestroyShipAction(const TransferableActionProps &props)
	: TransferActionBase(props)
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
		std::cerr << err.what() << "\n";
	}
}

void DestroyShipAction::play()
{
	if (!m_ship)
        return;
	if (GameManager::exist())
	{
		GameManager::session()->field.remove(m_ship);
	}
}

rn::Json DestroyShipAction::toJson() const
{
	return { };
}
