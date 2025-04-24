#pragma once

#include "decl.hpp"

struct GameObject
{
	size_t id;
public:
	GameObject();

	size_t getGameObjectId() const;
	virtual ~GameObject() = 0;
};
