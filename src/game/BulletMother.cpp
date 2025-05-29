#include "game/BulletMother.hpp"

ChildBullet::ChildBullet(BulletMother *mother, Bullet *bullet)
	: bullet(bullet),
	  mother(mother)

{
}

void ChildBullet::update()
{
	using rn::math::length;
	if (bullet && mother)
	{
		if (clock.is_stopped() && isOutsideViewArea())
		{
			clock.start();
		}
		else if (!clock.is_stopped()
				 && static_cast<float>(clock.time<std::chrono::milliseconds>().count()) > life_time_ms)
		{
			clock.stop();
			if (isOutsideViewArea())
				need_to_remove = true;
			else
				clock.reset();
		}
		bullet->update();
	}
}

const Bullet *ChildBullet::get() const
{
	return bullet.get();
}

void ChildBullet::start()
{
	if (bullet)
		bullet->start();
}
void ChildBullet::onEvent(sf::Event &event)
{
	if (bullet)
		bullet->onEvent(event);
}

bool ChildBullet::operator==(const Bullet &bullet) const
{
	return this->bullet.get() == &bullet;
}

bool ChildBullet::operator!=(const Bullet &bullet) const
{
	return !operator==(bullet);
}
sf::View BulletMother::getViewArea() const
{
	if (!camera)
		return { {}, rn::Vec2f(rn::VideoSettings::getResolution()) };
	return camera->getView();
}
bool ChildBullet::isOutsideViewArea() const
{
	if (!mother)
		return false;
	sf::View view = mother->getViewArea();
	sf::FloatRect view_area(view.getCenter() - view.getSize() / 2.f, view.getSize());
	return !view_area.contains(bullet->getPosition());
}
BulletMother::BulletMother(const Camera2d *camera)
	: camera(camera)
{
}
void BulletMother::setCamera(const Camera2d *camera2d)
{
	camera = camera2d;
}

const Camera2d *BulletMother::getCamera() const
{
	return camera;
}
void BulletMother::summon(Bullet *bullet)
{
	if (!bullet)
		return;

	emplace_back(this, bullet);
}
void BulletMother::update()
{
	std::vector<const_iterator> remove_bullet_stack{};
	for (auto child_bullet = begin(); child_bullet != end(); ++child_bullet)
	{
		child_bullet->update();
		if (child_bullet->need_to_remove)
			remove_bullet_stack.push_back(child_bullet);
	}

	for (auto &iterator: remove_bullet_stack)
		erase(iterator);
}

void BulletMother::start()
{
	for (auto &iterator: *this)
		iterator.start();
}

void BulletMother::onEvent(sf::Event &event)
{
	for (auto &iterator: *this)
		iterator.onEvent(event);
}

void BulletMother::destroy(const Bullet *bullet)
{
	auto it = std::ranges::find_if(*this, [&](const ChildBullet &blt) {
		return blt.get() == bullet;
	});
	if (it != end())
		erase(it);
}

