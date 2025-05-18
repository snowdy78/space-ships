#pragma once

#include "GameObjectFactory.hpp"

template<class T>
class GameObjectBase : public virtual GameObject
{
	static size_t identify();

public:
	using GameObject::GameObject;
	~GameObjectBase() override;
	inline static const size_t identifier = identify();
};


template<class T>
GameObjectBase<T>::~GameObjectBase() = default;


template<class T>
size_t GameObjectBase<T>::identify()
{
	return GameObjectFactory::instance().push<T>();
}
