#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"
#include "game/Bullet.hpp"

class DestroyBulletAction : public TransferActionBase<DestroyBulletAction>
{
    Bullet *m_bullet{nullptr};
    Hittable *m_contributor{nullptr};
public:
    /**
     * @brief Construct a new Destroy Bullet Action object
     * 
     * @param author is a bullet 
     * @param contributor is a hittable object
     * @param props
     */
	DestroyBulletAction(const TransferableActionProps &props);
    void play() override;
	rn::Json toJson() const override;
};

