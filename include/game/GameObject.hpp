#pragma once

#include "decl.hpp"

class GameObject
{
	size_t id;
	template<class T>
	friend class GameObjectBase;

public:
	GameObject();
	size_t hash() const;
	virtual ~GameObject() = 0;
};
