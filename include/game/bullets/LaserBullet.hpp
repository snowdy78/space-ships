#pragma once

#include "game/FirearmBullet.hpp"
#include "components/effects/HitSparkle.hpp"

constexpr char laser_bullet_sprite_path[] = "img/laser-bullet.png";
constexpr char laser_bullet_class_name[]  = "LaserBullet";

struct LaserBullet final : FirearmBullet<LaserBullet, HitSparkle, laser_bullet_sprite_path, laser_bullet_class_name>
{
};
