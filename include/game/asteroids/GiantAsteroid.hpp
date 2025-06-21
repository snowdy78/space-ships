#pragma once

#include "AsteroidBase.hpp"

constexpr char giant_asteroid_class_name[] = "GiantAsteroid";
constexpr char giant_asteroid_tex_path[]	= "img/asteroid3.png";

struct GiantAsteroid final : AsteroidBase<GiantAsteroid, giant_asteroid_class_name, giant_asteroid_tex_path>
{
};
