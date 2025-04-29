#include "components/EffectManager.hpp"

EffectManager::const_iterator EffectManager::begin() { return m_effects.begin(); }

EffectManager::const_iterator EffectManager::end() { return m_effects.end(); }

EffectManager::const_iterator EffectManager::erase(const_iterator index) 
{
    return m_effects.erase(index);
}

size_t EffectManager::size() const 
{
    return m_effects.size();
}

void EffectManager::clear() 
{
    m_effects.clear();
}

void EffectManager::draw(sf::RenderTarget &target, sf::RenderStates states) const 
{
    for (auto &effect : m_effects)
    {
        if (effect)
        {
            target.draw(*effect, states);
        }
    }
}

void EffectManager::update() 
{
    for (auto effect = begin(); effect != end(); ++effect)
    {
        if (*effect)
        {
            if ((*effect)->played())
                effect = --erase(effect);
            else 
                (*effect)->update();
        }
    }
}
