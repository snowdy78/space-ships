#include "components/EffectManager.hpp"

bool EffectManager::need_erase(const const_iterator &value) const 
{
    return (*value)->played();
}

void EffectManager::update_item(const const_iterator &value) 
{
    (*value)->update();
}

void EffectManager::onPushValue(const const_iterator &value)
{
    (*value)->start();
}

void EffectManager::draw(sf::RenderTarget &target, sf::RenderStates states) const 
{
    for (auto &value : *this)
    {
        target.draw(*value, states);
    }
}
