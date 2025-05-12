#pragma once

#include "decl.hpp"
#include "Transferable.hpp"

template<class T>
concept TransferObjectConcept = is_fabric_type_v<TransferableObject, T> && requires (T *ptr, T value)
{
	ptr = new T();
}; 

template<class T>
class BaseTransferableObject;

class TransferableObject : public Transferable
{
    template<class T>
	friend class BaseTransferableObject;

	template<TransferObjectConcept T>
	constexpr static void identify()
	{
	}
	TransferableObject() = default;

protected:
    using IdentifierT = void(*)();

public:
    ~TransferableObject() override = 0;

	virtual void receiveJson(const rn::Json &json) {}
};

template<class T>
class BaseTransferableObject : public TransferableObject
{
	static bool emplaceToFabric();
	constexpr static IdentifierT cs_id = &identify<T>;
	inline static bool emplaced_to_fabric = emplaceToFabric();

public:
	static size_t id();
};

class TransferableObjectFabric 
{
    using create_func = std::function<std::unique_ptr<TransferableObject>()>;
    std::unordered_map<size_t, create_func> transfer_objects{};
    TransferableObjectFabric() = default;
protected:
	void erase(size_t id);
    void clear();
    template<class T>
		requires(std::is_base_of_v<BaseTransferableObject<T>, T>)
	void push() noexcept;
	template<class T>
	friend class BaseTransferableObject;

public:
    static TransferableObjectFabric &instance();

    const create_func &get(size_t id);
};

template<class T>
bool BaseTransferableObject<T>::emplaceToFabric()
{
	TransferableObjectFabric::instance().push<T>();
	return true;
}

template<class T>
size_t BaseTransferableObject<T>::id()
{
	return reinterpret_cast<size_t>(cs_id);
}

template<class T>
	requires(std::is_base_of_v<BaseTransferableObject<T>, T>)
void TransferableObjectFabric::push() noexcept 
{
	transfer_objects.emplace(T::id(), []() {
		return std::unique_ptr<TransferableObject>{ new T() };
	});
}

