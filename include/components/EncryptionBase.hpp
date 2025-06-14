#pragma once

#include "decl.hpp"

template<class Ty, class To = Ty>
	requires(std::is_convertible_v<Ty, To> && std::is_convertible_v<To, Ty>)
class EncryptionBase
{
public:
	virtual ~EncryptionBase() = default;
	virtual To encrypt(const Ty &value, size_t key) const
	{
		return value;
	}
	virtual Ty decrypt(const To &value, size_t key) const
	{
		return value;
	}
};
