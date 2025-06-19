#pragma once

#include "game/FirearmWeapon.hpp"
#include "game/bullets/LittleBullet.hpp"

constexpr char minigun_sound_path[] = "shoot.ogg";
constexpr char minigun_sprite_path[] = "img/minigun.png";
constexpr char minigun_class_name[]	 = "MiniGun";

struct MiniGun final : FirearmWeapon<MiniGun, LittleBullet, minigun_sound_path, minigun_sprite_path, minigun_class_name>
{};
