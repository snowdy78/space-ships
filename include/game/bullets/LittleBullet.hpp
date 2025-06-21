#pragma once

#include "decl.hpp"
#include "game/FirearmBullet.hpp"
#include "components/effects/HitSparkle.hpp"

constexpr char base_bullet_sprite_path[] = "img/small-bullet.png";
constexpr char base_bullet_class_name[]  = "LittleBullet";

struct LittleBullet final : FirearmBullet<LittleBullet, HitSparkle, base_bullet_sprite_path, base_bullet_class_name>
{
};
