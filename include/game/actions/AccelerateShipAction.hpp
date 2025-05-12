#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

class AccelerateShipAction : public BaseTransferableAction<AccelerateShipAction>
{
public:
    constexpr static const char *acceleration = "acceleration";
	explicit AccelerateShipAction(const TransferableActionProps &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    AbstractShip *m_ship{nullptr}; // author
    float m_acceleration{};
};
