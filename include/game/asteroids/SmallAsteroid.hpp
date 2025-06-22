#pragma once

#include "AsteroidBase.hpp"

constexpr char small_asteroid_class_name[] = "SmallAsteroid";
constexpr char small_asteroid_tex_path[]   = "img/asteroid2.png";
constexpr char small_asteroid_hit_path[]   = "assets/asteroid-hit.png";

struct SmallAsteroid final : AsteroidBase<SmallAsteroid, small_asteroid_class_name, small_asteroid_tex_path, small_asteroid_hit_path>
{
};
