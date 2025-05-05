#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

class AccelerateShipAction : public TransferableAction
{
public:
    constexpr static const char *acceleration = "acceleration";
    explicit AccelerateShipAction(GameObject *author = nullptr, GameObject *contributor = nullptr, const rn::Json &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    static const size_t id;
    AbstractShip *m_ship{nullptr}; // author
    float m_acceleration{};
};
