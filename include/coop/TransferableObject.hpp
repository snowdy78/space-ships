#pragma once

#include "decl.hpp"
#include "Transferable.hpp"

template<class T>
concept TransferObjectConcept = is_fabric_type_v<TransferableObject, T> && requires (T *ptr, T value)
{
	ptr = new T();
}; 

class TransferableObject : public Transferable
{
	TransferableObject() = default;
	template<class T>
	friend class TransferObjectBase;

protected:
    template<TransferObjectConcept T>
	static size_t identify();

public:
    ~TransferableObject() override = 0;

	virtual void receiveJson(const rn::Json &json) {}
};

template<class T>
class TransferObjectBase : public TransferableObject
{
public:
	~TransferObjectBase() override = 0;
	inline static const size_t identifier = identify<T>();
};
class TransferableObjectFabric 
{
    using create_func = std::function<std::unique_ptr<TransferableObject>()>;
    std::unordered_map<size_t, create_func> transfer_objects{};
	size_t id_encounter = 0;
    TransferableObjectFabric() = default;
public:
    static TransferableObjectFabric &instance();

    const create_func &get(size_t id) const;
    template<class T>
    size_t push();
};

template<class T>
size_t TransferableObjectFabric::push() 
{
    transfer_objects.emplace(id_encounter, []() { return std::unique_ptr<TransferableObject>{new T()}; });
	std::cout << "id_encounter: " << id_encounter << typeid(T).name() << "\n";
	return id_encounter++;
}


template<TransferObjectConcept T>
size_t TransferableObject::identify()
{
	return TransferableObjectFabric::instance().push<T>();
}

template<class T>
TransferObjectBase<T>::~TransferObjectBase() = default;
