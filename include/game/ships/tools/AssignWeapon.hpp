#pragma once

#include "game/ships/AbstractShip.hpp"
#include "game/GameManager.hpp"

inline void shipTakeWeapon(AbstractShip &ship, const SpaceField::StatePtr<SpaceItem> &ptr)
{
	if (GameManager::exist())
	{
		auto &field						 = GameManager::session()->field;
		SpaceField::ItemConstIterator it = field.find(ptr);
		if (it != field.items().end())
		{
			ship.gun = std::dynamic_pointer_cast<AbstractWeapon>(field.take(it));
			if (!ship.existOnField())
				std::cerr << "ship not exist on field yet\n";
			ship.gun->assignOwner(ship.self());
		}
		else
			std::cerr << "cannot assign gun to ship. Item not found.\n";
	}
	else
	{
		std::cerr << "cannot assign gun to ship. Game session does not exist!\n";
	}
}

template<WeaponConcept T, class... Args>
void makeWeaponForShip(AbstractShip &ship, Args &&...args)
{
	if (GameManager::exist())
	{
		auto &field = GameManager::session()->field;
		auto ptr	= field.emplaceItem<T>(std::forward<Args>(args)...);
		::shipTakeWeapon(ship, ptr);
	}
	else
	{
		std::cerr << "cannot assign gun to ship. Game session does not exist!\n";
	}
}
