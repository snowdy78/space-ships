#pragma once

#include "decl.hpp"
#include "game/FirearmBullet.hpp"
#include "components/effects/HitSparkle.hpp"

constexpr char little_bullet_sprite_path[] = "img/small-bullet.png";
constexpr char little_bullet_class_name[]  = "LittleBullet";

struct LittleBullet final : FirearmBullet<LittleBullet, HitSparkle, little_bullet_sprite_path, little_bullet_class_name>
{
};
