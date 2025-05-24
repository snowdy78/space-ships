#include "game/GameObject.hpp"
#include "game/GameObjectFactory.hpp"

GameObject::GameObject()
	: m_id(GameObjectFactory::instance().push(this))
{
}

size_t GameObject::id() const
{
	return m_id;
}

GameObject::~GameObject()
{
	GameObjectFactory::instance().erase(m_id);
}

rn::Json GameObject::toJson() const
{
	return {
		{ names::object_id, id() }
	};
}
