#include "coop/Transferable.hpp"

Transferable::~Transferable() = default;

Transferable::Type::operator std::string() const
{
	return type;
};

bool Transferable::Type::operator==(const Type &other) const
{
	return type == other.type;
}

bool Transferable::Type::operator!=(const Type &other) const
{
	return type != other.type;
}

