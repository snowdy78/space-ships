#pragma once

#include "TransferableObject.hpp"
#include "decl.hpp"


struct Request : public TransferableObject, public rn::Json
{
	static const size_t id;
public:
    using rn::Json::Json;
    Request() = default;
    void receiveJson(const rn::Json &json) override;
	TransferJson toJson() const override;
};
