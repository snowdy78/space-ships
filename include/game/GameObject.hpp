#pragma once

#include "decl.hpp"

struct GameObject
{
	size_t id;
public:
	GameObject();

	virtual ~GameObject() = 0;
};
