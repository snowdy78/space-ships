#pragma once

#include "game/FirearmBullet.hpp"
#include "components/effects/HitSparkle.hpp"
constexpr char base_bullet_sprite_path[] = "img/bullet_shoot.png";
constexpr char base_bullet_class_name[]  = "BaseBullet";

struct BaseBullet final : FirearmBullet<BaseBullet, HitSparkle, base_bullet_sprite_path, base_bullet_class_name>
{
};
