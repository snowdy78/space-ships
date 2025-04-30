#include "components/SoundManager.hpp"

bool SoundManager::need_erase(const const_iterator &value) const
{
    return (*value)->getStatus() == sf::Sound::Stopped;
}

void SoundManager::update_item(const const_iterator &value)
{
    (*value)->update();
}

void SoundManager::onPushValue(const const_iterator &value)
{
    (*value)->play();
}
