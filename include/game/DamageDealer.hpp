#pragma once

#include "decl.hpp"
#include "game/GameObject.hpp"

class DamageDealer : public virtual GameObject
{
    float damage;
protected:
	void setDamage(float damage);
	virtual void onDealDamage(Hittable *, float damage) {}
public:
	DamageDealer(float damage = 0.f);
	void dealDamage(Hittable *hittable);
	float getDamage() const;
};
using init_dmg = DamageDealer;