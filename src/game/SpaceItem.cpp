#include "game/SpaceItem.hpp"
#include "coop/TransferableAction.hpp"
#include "game/GameManager.hpp"
#include "game/ships/AbstractShip.hpp"

void SpaceItem::updateCollider()
{
	float collider_radius = static_cast<float>(top_layer_anim.getKeyframe(0).getTexture().getSize().x) / 2.f;
	m_collider_widget.setRadius(collider_radius);
	m_collider_widget.setPosition(getPosition());
	m_collider_widget.setScale(getScale());
	m_collider_widget.setFillColor(default_collider_color);
	m_collider_widget.setOrigin(getOrigin());
	m_collider_widget.setRotation(getRotation());
	collider.transform(m_collider_widget);
}

void SpaceItem::assignOwner(OwnerPtrType owner)
{
	owner_ptr = std::move(owner);
	onOwnerAssign();
}

bool SpaceItem::hasOwner() const
{
	return !owner_ptr.expired();
}

void SpaceItem::start()
{
	if (!hasOwner())
	{
		top_layer_anim.setRepeating(true);
		bottom_layer_anim.setRepeating(true);
		top_layer_anim.assignKeyframesDuration(
			top_layer_anim.cbegin(), top_layer_anim.cend(),
			AnimatedSprite::time_digit_t(*props::top_layer_ms_frame_time)
		);
		bottom_layer_anim.assignKeyframesDuration(
			bottom_layer_anim.cbegin(), bottom_layer_anim.cend(),
			AnimatedSprite::time_digit_t(*props::bottom_layer_ms_frame_time)
		);
		top_layer_anim.start();
		bottom_layer_anim.start();
		updateCollider();
	}
}
void SpaceItem::update()
{
	if (!hasOwner())
	{
		top_layer_anim.update();
		bottom_layer_anim.update();
		if (GameManager::exist())
		{
			auto player_ptr = GameManager::session()->player;
			if (player_ptr.expired())
				return;
			auto player = player_ptr.lock();
			
			if (player->getCollider()->collide(collider))
				onPlayerTake(player);
		}
	}
}

void SpaceItem::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (!hasOwner())
	{
		states.transform *= getTransform();
		target.draw(bottom_layer_anim, states);
		target.draw(getItemSprite(), states);
		target.draw(top_layer_anim, states);
		if (*props::collider_visible)
		{
			target.draw(m_collider_widget);
		}
	}
}

void SpaceItem::onOwnerAssign()
{
}

