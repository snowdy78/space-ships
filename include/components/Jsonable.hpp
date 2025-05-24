#pragma once

struct Jsonable
{
	virtual ~Jsonable()				= default;
	virtual rn::Json toJson() const = 0;
};