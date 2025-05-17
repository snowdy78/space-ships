#include "game/actions/DestroyShipAction.hpp"
#include "game/AbstractShip.hpp"
#include "game/DamageDealer.hpp"
#include "game/GameManager.hpp"

const size_t DestroyShipAction::id = identify<DestroyShipAction>();

DestroyShipAction::DestroyShipAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: TransferableAction(author, contributor, props)
{
	if (auto ship = dynamic_cast<AbstractShip *>(author))
		m_ship = ship;
	try
	{
		if (auto destroyer = dynamic_cast<DamageDealer *>(contributor))
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
	if (GameManager::exist())
	{
		GameManager::instance().field.remove(m_ship);
	}
}

DestroyShipAction::TransferJson DestroyShipAction::toJson() const
{
	return { id };
}

AbstractAction *DestroyShipAction::copy() const
{
    return new DestroyShipAction(m_ship, m_destroyer);
}
