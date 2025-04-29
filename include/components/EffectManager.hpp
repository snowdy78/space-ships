#pragma once

#include <type_traits>
#include "Effect.hpp"

template<class T>
concept EffectConcept = std::is_base_of_v<Effect, T> && !std::is_same_v<T, Effect>;

class EffectManager : public sf::Drawable, public rn::LogicalObject
{
public:
    template<class T>
    using container = std::vector<T>;
    using effect_t = std::unique_ptr<Effect>;
    using effects_container = container<effect_t>;
    using const_iterator = effects_container::const_iterator; 

    const_iterator begin();
    const_iterator end();
    EffectManager() = default;

    template<EffectConcept E, class ...Args>
    void emplace_back(const rn::Vec2f &position, Args &&...args);
    template<EffectConcept E, class ...Args>
    void emplace_back(const rn::Vec2f &position, const Args &...args);
    const_iterator erase(const_iterator index);
    size_t size() const;
    void clear();

    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::vector<effect_t> m_effects;
};

template<EffectConcept E, class ...Args>
void EffectManager::emplace_back(const rn::Vec2f &position, Args &&...args) 
{
    m_effects.emplace_back(nullptr);
    m_effects.back().reset(new E(std::move(args)...));
    m_effects.back()->setPosition(position);
    m_effects.back()->start();
}

template<EffectConcept E, class ...Args>
void EffectManager::emplace_back(const rn::Vec2f &position, const Args &...args) 
{
    m_effects.emplace_back(nullptr);
    m_effects.back().reset(new E(args...));
    m_effects.back()->setPosition(position);
    m_effects.back()->start();
}

