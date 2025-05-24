#pragma once

#include "decl.hpp"
#include "components/Jsonable.hpp"

class GameObject : public virtual Jsonable
{
	size_t m_id;
	template<class T>
	friend class GameObjectBase;

public:
	struct names
	{
		static constexpr const char *object_id = "object_id";
	};
	GameObject();
	size_t id() const;
	~GameObject() override = 0;
	rn::Json toJson() const override;
};
