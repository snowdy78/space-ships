#pragma once

#pragma once

#include "game/guns/LaserRifle.hpp"
#include "game/ships/EnemyShipBase.hpp"

constexpr char enemy_ship_class_name[]	= "EnemyShip";
constexpr char enemy_ship_sprite_path[] = "img/enemy_ship.png";

struct EnemyShip final : EnemyShipBase<EnemyShip, LaserRifle, enemy_ship_class_name, enemy_ship_sprite_path>
{
};
