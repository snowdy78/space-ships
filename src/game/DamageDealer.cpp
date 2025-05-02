#include "game/DamageDealer.hpp"

DamageDealer::DamageDealer(float damage)
	: damage(damage)
{}
void DamageDealer::dealDamage(Hittable *hittable) 
{
	onDealDamage(hittable, damage);
}
float DamageDealer::getDamage() const
{
	return damage;
}
void DamageDealer::setDamage(float damage)
{
	this->damage = damage;
}
