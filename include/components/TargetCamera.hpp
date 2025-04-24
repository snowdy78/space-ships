#pragma once

#include "decl.hpp"
#include "game/Camera2d.hpp"

class TargetCamera : public Camera2d
{
    sf::RenderTarget &target;
    std::function<void()> update_on_move;

public:
    TargetCamera(sf::RenderTarget &target, std::function<void()> update_on_move)
        : target(target), update_on_move(update_on_move)
    {

    }
    virtual void onCameraMove() override
    {
        target.setView(getView());
        update_on_move();    
    }
};

