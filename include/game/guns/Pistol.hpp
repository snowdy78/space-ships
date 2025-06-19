#pragma once

#include "game/FirearmWeapon.hpp"
#include "game/bullets/BaseBullet.hpp"

constexpr char pistol_sound_path[] = "shoot.ogg";
constexpr char pistol_sprite_path[] = "img/pistol.png";
constexpr char pistol_class_name[]	= "Pistol";

struct Pistol final : FirearmWeapon<Pistol, BaseBullet, pistol_sound_path, pistol_sprite_path, pistol_class_name>
{
	
};
