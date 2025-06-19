#pragma once

#include "game/FirearmBullet.hpp"

constexpr char base_bullet_sprite_path[] = "img/bullet_shoot.png";
constexpr char base_bullet_class_name[]  = "BaseBullet";

struct BaseBullet final : FirearmBullet<BaseBullet, base_bullet_sprite_path, base_bullet_class_name>
{
};
