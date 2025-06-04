#pragma once

#include "decl.hpp"

struct Jsonable
{
	virtual ~Jsonable()				= default;
	virtual rn::Json toJson() const = 0;
};