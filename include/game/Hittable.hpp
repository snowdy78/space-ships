#pragma once

#include "components/FileLoader.hpp"
#include "decl.hpp"
#include "GameObject.hpp"

class Hittable : public virtual GameObject
{
	float health	 = 100.f;
	float max_health = 100.f;

protected:
	friend class HealthBar;
	class HealthBar : public sf::Drawable, public sf::Transformable
	{
		inline static loading<sf::Texture> texture = FileLoader::Instance().addTextureToUpload("img/health_bar.png").get();
		rn::Rect ui;
		rn::Vec2f bar_size = { 30, 6 };
		rn::Rect fill_bar;
		rn::Rect back_bar;
		const Hittable &hittable;
		void updateHealthBar();
		friend class Hittable;

	public:
		HealthBar(const Hittable &hittable);
		void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	};
	HealthBar health_bar{ *this };

public:
	~Hittable() override;
	float getMaxHealth() const;
	void takeDamage(float damage);
	float getHealth() const;
	bool isDead() const;
protected:
	virtual void onHit();
};
