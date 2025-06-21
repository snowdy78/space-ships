#pragma once

#include "game/ships/EnemyShipBase.hpp"
#include "game/guns/Pistol.hpp"

constexpr char enemy_ship_class_name[] = "EnemyShip";
constexpr char enemy_ship_sprite_path[] = "img/enemy_ship.png";

struct EnemyShip final : EnemyShipBase<EnemyShip, Pistol, enemy_ship_class_name, enemy_ship_sprite_path>
{
};
