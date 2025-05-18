#pragma once

#include "decl.hpp"

class Session
{
public:
	virtual ~Session()					   = 0;
	virtual const rn::Json &toJson() const = 0;
};
