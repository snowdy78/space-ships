#include "game/Hittable.hpp"

Hittable::HealthBar::HealthBar(const Hittable &hittable)
	: ui({ 32, 8 }),
	  hittable(hittable)
{
	ui.setTexture(texture);
	updateHealthBar();
	fill_bar.setPosition(1, 1);
	fill_bar.setFillColor(sf::Color::Green);
	back_bar.setFillColor(sf::Color::Red);
}
void Hittable::HealthBar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(back_bar, states);
	target.draw(fill_bar, states);
	target.draw(ui, states);
}
void Hittable::HealthBar::updateHealthBar()
{
	float health_bar_width = hittable.health * bar_size.x / hittable.max_health;
	fill_bar.setSize({ health_bar_width, bar_size.y });
	back_bar.setSize({ bar_size.x - health_bar_width, bar_size.y });
	back_bar.setPosition(fill_bar.getPosition().x + fill_bar.getSize().x, 1);
}
float Hittable::getMaxHealth() const
{
	return max_health;
}
void Hittable::takeDamage(float damage)
{
	health = std::clamp(health - damage, 0.f, max_health);
	onHit();
}
float Hittable::getHealth() const
{
	return health;
}

bool Hittable::isDead() const
{
	return getHealth() <= 0;
}

void Hittable::onHit()
{
	health_bar.updateHealthBar();
}
