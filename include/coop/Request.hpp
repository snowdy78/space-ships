#pragma once

#include "decl.hpp"
#include "TransferableObject.hpp"

struct Request : public TransferableObject
{
    static const size_t id;
    TransferJson object;
public:
    Request(const rn::Json &data = {});
    rn::Json &at(const char *key);
    const rn::Json &at(const char *key) const;
    TransferJson toJson() const override;
};



