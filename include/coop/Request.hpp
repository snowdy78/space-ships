#pragma once

#include "TransferableObject.hpp"
#include "decl.hpp"


struct Request : BaseTransferableObject<Request>, rn::Json
{
    using rn::Json::Json;
    Request() = default;
    void receiveJson(const rn::Json &json) override;
	TransferJson toJson() const override;
};
