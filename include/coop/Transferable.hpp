#pragma once

#include "decl.hpp"
#include "TransferableFabric.hpp"

class Transferable
{
public:
	class Type
	{
		std::string type;
		Type(const std::string &type)
			: type(type)
		{}
		friend class Transferable;
	public:
		operator std::string() const;
		bool operator==(const Type &other) const;
		bool operator!=(const Type &other) const;
	};

private:
	Type _type;

protected:
	template<class T>
	static size_t identify();

public:
	inline static Type action{ "action" };
	inline static Type object{ "object" };
	Transferable(Type type) : _type(type) {}
	virtual ~Transferable()			= 0;
	virtual rn::Json toJson() const = 0;
	Type type() const
	{
		return _type;
	}
};
template<class T>
size_t Transferable::identify()
{
	return TransferableFabric::instance().push<T>();
}
