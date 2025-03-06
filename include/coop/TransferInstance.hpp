#pragma once

#include "decl.hpp"

class TransferInstance : public rn::Json
{
public:
    using rn::Json::Json;
    TransferInstance(const rn::Json &json)
        : rn::Json(json) {}
    TransferInstance &operator=(const rn::Json &json);
};