#pragma once

#include "TransferInstance.hpp"
#include "decl.hpp"

class TransferObject
{
	inline static std::unordered_map<size_t, std::function<TransferObject *()>> transfer_objects{};

protected:
	template<class T>
	static size_t identify();

public:
	virtual ~TransferObject()				= 0;
	virtual TransferInstance toJson() const = 0;
};
template<class T>
size_t TransferObject::identify()
{
	transfer_objects.insert({ transfer_objects.size(), []() -> TransferObject * {
								 return new T();
							 } });
	return transfer_objects.size() - 1;
}
