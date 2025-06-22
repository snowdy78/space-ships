#pragma once

#include "game/guns/LaserRifle.hpp"
#include "game/ships/EnemyShipBase.hpp"

constexpr char demonic_ship_class_name[]  = "DemonicEnemyShip";
constexpr char demonic_ship_sprite_path[] = "img/ship-demonic.png";

struct DemonicEnemyShip final
	: EnemyShipBase<DemonicEnemyShip, LaserRifle, demonic_ship_class_name, demonic_ship_sprite_path>
{
};
