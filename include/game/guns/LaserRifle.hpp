#pragma once

#include "game/FirearmWeapon.hpp"
#include "game/bullets/LaserBullet.hpp"

constexpr char laser_rifle_sound_path[]	= "shoot.ogg";
constexpr char laser_rifle_sprite_path[] = "img/rifle.png";
constexpr char laser_rifle_class_name[]	 = "LaserRifle";

struct LaserRifle final : FirearmWeapon<LaserRifle, LaserBullet, laser_rifle_sound_path, laser_rifle_sprite_path, laser_rifle_class_name>
{
};
