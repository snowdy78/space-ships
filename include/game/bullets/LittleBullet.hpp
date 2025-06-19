#pragma once

#include "decl.hpp"
#include "game/FirearmBullet.hpp"

constexpr char base_bullet_sprite_path[] = "img/small-bullet.png";
constexpr char base_bullet_class_name[]  = "LittleBullet";

struct LittleBullet final : FirearmBullet<LittleBullet, base_bullet_sprite_path, base_bullet_class_name>
{
};
