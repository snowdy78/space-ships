#pragma once

#include "AsteroidBase.hpp"

constexpr char simple_asteroid_class_name[] = "SimpleAsteroid";
constexpr char simple_asteroid_tex_path[]	= "img/asteroid.png";

struct SimpleAsteroid final : AsteroidBase<SimpleAsteroid, simple_asteroid_class_name, simple_asteroid_tex_path>
{
};
