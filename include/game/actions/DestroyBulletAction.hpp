#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"
#include "game/Bullet.hpp"

class DestroyBulletAction : public TransferableAction
{
    const static size_t id;
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
    DestroyBulletAction(GameObject *author = nullptr, GameObject *contributor = nullptr, const rn::Json &props = {});
    void play() override;
    virtual TransferJson toJson() const override;
    virtual AbstractAction *copy() const override;
};

