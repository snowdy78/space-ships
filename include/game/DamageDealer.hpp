#pragma once

#include "SpaceFieldObject.hpp"
#include "decl.hpp"

class DamageDealer : public virtual SpaceFieldObject
{
	float damage;

protected:
	void setDamage(float damage);
	virtual void onDealDamage(Hittable *, float damage);
public:
	DamageDealer(float damage = 0.f);
	~DamageDealer() override;
	void dealDamage(Hittable *hittable);
	float getDamage() const;
};
using init_dmg = DamageDealer;
