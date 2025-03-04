#pragma once

#include "TransferInstance.hpp"
#include "decl.hpp"

class TransferObject
{
public:
    virtual ~TransferObject() = 0;
    virtual TransferInstance toJson() const = 0;
};
