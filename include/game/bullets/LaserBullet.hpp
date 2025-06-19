#pragma once

#include "game/FirearmBullet.hpp"

constexpr char laser_bullet_sprite_path[] = "img/small-bullet.png";
constexpr char laser_bullet_class_name[]  = "LittleBullet";

struct LaserBullet final : FirearmBullet<LaserBullet, laser_bullet_sprite_path, laser_bullet_class_name>
{
};
