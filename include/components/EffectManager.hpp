#pragma once

#include "decl.hpp"
#include "Effect.hpp"

class EffectManager : public sf::Drawable, public rn::LogicalObject
{
public:
    template<class T>
    using container = std::vector<T>;
    using effect_t = const Effect *;
    using effects_container = container<effect_t>;
    using const_iterator = effects_container::const_iterator; 

    const_iterator begin();
    const_iterator end();
    EffectManager() = default;

    void push_back(const Effect &sprite);
    const_iterator erase(const_iterator index);
    size_t size() const;
    void clear();

    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::vector<effect_t> m_effects;
};

