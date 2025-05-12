#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"
#include "game/Bullet.hpp"

class DestroyBulletAction : public BaseTransferableAction<DestroyBulletAction>
{
public:
    /**
     * @brief Construct a new Destroy Bullet Action object
     * 
     * @param author is a bullet 
     * @param contributor is a hittable object
     * @param props
     */
	DestroyBulletAction(const TransferableActionProps &props = {});
    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
	Bullet *m_bullet{ nullptr };
	Hittable *m_contributor{ nullptr };
};

