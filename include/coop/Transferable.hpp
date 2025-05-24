#pragma once

#include "decl.hpp"
#include "components/Jsonable.hpp"

class Transferable : public virtual Jsonable
{
	class TransferData : public rn::Json
	{
	public:
		TransferData(size_t id, const rn::Json &data = {});
		size_t id() const;
		const rn::Json &data() const;
		rn::Json &data();
	};

public:
	using TransferJson					= TransferData;
	Transferable()						= default;
	virtual ~Transferable()				= 0;
	virtual TransferJson requestData() const = 0;
};
