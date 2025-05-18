#include "game/BulletMother.hpp"

BulletMother::ChildBullet::ChildBullet(BulletMother *mother, Bullet *bullet)
	: bullet(bullet),
	  mother(mother)

{}

void BulletMother::ChildBullet::update()
{
	using rn::math::length;
	if (bullet && mother)
	{
		if (clock.is_stopped() && isOutsideViewArea())
		{
			clock.start();
		}
		else if (!clock.is_stopped() && static_cast<float>(clock.time<std::chrono::milliseconds>().count()) > life_time_ms)
		{
			clock.stop();
			if (isOutsideViewArea())
			{
				need_to_remove = true;
			}
			else
			{
				clock.reset();
			}
		}
		bullet->update();
	}
}

const Bullet *BulletMother::ChildBullet::get() const
{
	return bullet.get();
}
sf::View BulletMother::getViewArea() const
{
	if (!camera)
		return { {}, rn::Vec2f(rn::VideoSettings::getResolution()) };
	return camera->getView();
}
bool BulletMother::ChildBullet::isOutsideViewArea() const
{
	if (!mother)
		return false;
	sf::View view = mother->getViewArea();
	sf::FloatRect view_area(view.getCenter() - view.getSize() / 2.f, view.getSize());
	return !view_area.contains(bullet->getPosition());
}
BulletMother::BulletMother(const Camera2d *camera)
	: camera(camera)
{}

BulletMother::iterator BulletMother::begin()
{
	return bullets.begin();
}

BulletMother::iterator BulletMother::end()
{
	return bullets.end();
}

void BulletMother::setCamera(const Camera2d *camera2d)
{
	camera = camera2d;
}

const Camera2d *BulletMother::getCamera() const
{
	return camera;
}

BulletMother::const_iterator BulletMother::cbegin() const
{
	return bullets.cbegin();
}

BulletMother::const_iterator BulletMother::cend() const
{
	return bullets.cend();
}

BulletMother::const_iterator BulletMother::begin() const
{
	return bullets.begin();
}

BulletMother::const_iterator BulletMother::end() const
{
	return bullets.end();
}

size_t BulletMother::bulletCount() const
{
	return bullets.size();
}

void BulletMother::summon(Bullet *bullet)
{
	if (!bullet)
		return;

	bullets.emplace_back(this, bullet);
}
void BulletMother::update()
{
	std::vector<const_iterator> remove_bullet_stack{};
	for (auto child_bullet = bullets.begin(); child_bullet != bullets.end(); ++child_bullet)
	{
		child_bullet->update();
		if (child_bullet->need_to_remove)
			remove_bullet_stack.push_back(child_bullet);
	}

	for (auto &iterator: remove_bullet_stack)
	{
		bullets.erase(iterator);
	}
}
void BulletMother::ChildBullet::start()
{
	if (bullet)
		bullet->start();
}
void BulletMother::ChildBullet::onEvent(sf::Event &event)
{
	if (bullet)
		bullet->onEvent(event);
}

bool BulletMother::ChildBullet::operator==(const Bullet &bullet) const 
{
	return this->bullet.get() == &bullet;
}

bool BulletMother::ChildBullet::operator!=(const Bullet &bullet) const 
{
	return !operator==(bullet);	
}

void BulletMother::start()
{
	for (auto &iterator: bullets)
	{
		iterator.start();
	}
}

void BulletMother::onEvent(sf::Event &event)
{
	for (auto &iterator: bullets)
	{
		iterator.onEvent(event);
	}
}

void BulletMother::destroy(const Bullet *bullet)
{
	auto it = std::ranges::find_if(bullets, [&](const ChildBullet &blt) {
		return blt.get() == bullet;
	});
	if (it != bullets.end())
	{
		bullets.erase(it);
	}
}
