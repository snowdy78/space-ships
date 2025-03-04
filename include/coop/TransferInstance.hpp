#pragma once

#include "decl.hpp"

class TransferInstance
{
public:
    rn::Json json;
    TransferInstance() = default;
    TransferInstance(const rn::Json &json) : json(json) {}
};