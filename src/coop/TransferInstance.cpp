#include "coop/TransferInstance.hpp"

TransferInstance &TransferInstance::operator=(const rn::Json &json)
{
    rn::Json::operator=(json);
    return *this;
}
