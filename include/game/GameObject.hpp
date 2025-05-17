#pragma once

#include "decl.hpp"

class GameObject
{
	size_t id;
public:
	GameObject();

	size_t getGameObjectId() const;
	virtual ~GameObject() = 0;
};
