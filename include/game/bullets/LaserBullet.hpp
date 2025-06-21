#pragma once

#include "game/FirearmBullet.hpp"
#include "components/effects/HitSparkle.hpp"

constexpr char laser_bullet_sprite_path[] = "img/small-bullet.png";
constexpr char laser_bullet_class_name[]  = "LittleBullet";

struct LaserBullet final : FirearmBullet<LaserBullet, HitSparkle, laser_bullet_sprite_path, laser_bullet_class_name>
{
};
