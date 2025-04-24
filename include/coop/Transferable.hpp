#pragma once

#include "decl.hpp"

class Transferable
{
	class TransferData : public rn::Json
	{
	public:
		TransferData(size_t id, const rn::Json &data = {});
		const size_t id() const;
		const rn::Json &data() const;
		rn::Json &data();
	};

public:
	using TransferJson					= TransferData;
	Transferable()						= default;
	virtual ~Transferable()				= 0;
	virtual TransferJson toJson() const = 0;
};
