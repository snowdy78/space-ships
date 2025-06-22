#pragma once

#include "game/guns/LaserRifle.hpp"
#include "game/ships/EnemyShipBase.hpp"

constexpr char redenemy_ship_class_name[]	= "RedEnemyShip";
constexpr char redenemy_ship_sprite_path[] = "img/ship-red.png";

struct RedEnemyShip final : EnemyShipBase<RedEnemyShip, LaserRifle, redenemy_ship_class_name, redenemy_ship_sprite_path>
{
};
