#include "game/AbstractAsteroid.hpp"
#include "game/GameManager.hpp"
#include "game/actions/DestroySpaceFieldObjectAction.hpp"

AbstractAsteroid::AbstractAsteroid(const sf::Texture &texture)
	: m_sprite(texture)
{
}

AbstractAsteroid::~AbstractAsteroid() = default;

rn::Vec2f AbstractAsteroid::getSize() const
{
	return m_sprite.getTexture() ? rn::Vec2f(m_sprite.getTexture()->getSize()) : rn::Vec2f{};
}

sf::FloatRect AbstractAsteroid::getLocalBounds() const
{
	return m_sprite.getLocalBounds();
}

sf::FloatRect AbstractAsteroid::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void AbstractAsteroid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}

void AbstractAsteroid::movement()
{
	using namespace std::chrono_literals;
	RigitBody2d::movement();
	m_transition.setMovingDelay(100ms);
}

void AbstractAsteroid::onMove()
{
	RigitBody2d::onMove();
	updateCollider();
}

void AbstractAsteroid::rotation()
{
	using namespace std::chrono_literals;
	RigitBody2d::rotation();
	m_transition.setRotating(0, 360);
	m_transition.setRotationDelay(std::chrono::milliseconds(static_cast<size_t>(getMass()*getAcceleration())));
	m_transition.repeat_rotation = true;
}

void AbstractAsteroid::start()
{
	RigitBody2d::start();
	setOrigin(getSize() / 2.f);
	setMass(m_density * static_cast<float>(rn::math::rectangle(getLocalBounds()).area()));
}

void AbstractAsteroid::update()
{
	RigitBody2d::update();
	m_transition.setMoving(getPosition(), getPosition() + countMove());
	m_transition.update();
	onMove();	
}


const Collider *AbstractAsteroid::getCollider() const
{
	return &collider;
}

void AbstractAsteroid::onCollisionEnter(Collidable *obstacle)
{
	if (GameManager::exist())
	{
		GameManager::session()->action_manager.emplaceToTop<DestroySpaceFieldObjectAction>(TransferableActionProps{
			this });
	}
}

bool AbstractAsteroid::resolve(const Collidable *obstacle) const
{
	return dynamic_cast<const SpaceFieldObject *>(obstacle);
}
