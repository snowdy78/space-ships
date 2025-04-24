#include "coop/Transferable.hpp"

Transferable::~Transferable() = default;

Transferable::TransferData::TransferData(size_t id, const rn::Json &data)
    : rn::Json({ {"id", id}, {"data", data} })
{
};

const size_t Transferable::TransferData::id() const
{
	return at("id");
}
const rn::Json &Transferable::TransferData::data() const
{
	return at("data");
}
rn::Json &Transferable::TransferData::data()
{
	return at("data");
}
