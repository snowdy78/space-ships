#pragma once

#include "decl.hpp"

class Camera2d : public sf::Transformable
{
	rn::Vec2f m_viewsize;

public:
	Camera2d();

    void setPosition(const rn::Vec2f &v);
    void setPosition(float x, float y);
    void move(const rn::Vec2f &v);
    void move(float x, float y);

    virtual void onCameraMove() {}

    rn::Vec2f getViewSize() const;    
	sf::View getView() const;
	sf::FloatRect getViewRect() const;
};
