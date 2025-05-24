#pragma once

#include "decl.hpp"
#include "Transferable.hpp"
#include "game/GameObjectBase.hpp"

template<class T>
concept TransferObjectConcept = is_fabric_type_v<TransferableObject, T> && requires (T *ptr, T value)
{
	ptr = new T();
}; 

class TransferableObject : public Transferable, public virtual GameObject
{
	TransferableObject() = default;
	template<class T>
	friend class TransferObjectBase;

public:
    ~TransferableObject() override = 0;

	virtual void receiveJson(const rn::Json &json) {}
};

template<class T>
class TransferObjectBase : public TransferableObject, public GameObjectBase<T>
{
public:
	~TransferObjectBase() override = 0;

	rn::Json toJson() const override
	{
		return GameObjectBase<T>::toJson();
	}
	TransferJson requestData() const override
	{
		return { GameObjectBase<T>::identifier, toJson() };
	}
};

template<class T>
TransferObjectBase<T>::~TransferObjectBase() = default;
