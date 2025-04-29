#pragma once

#include "decl.hpp"

class Effect : public rn::LogicalObject, public sf::Sprite
{
public:
	using time_digit_t = std::chrono::milliseconds;
    ~Effect() override = 0;
    virtual bool played() const = 0;
};

inline Effect::~Effect() = default;
