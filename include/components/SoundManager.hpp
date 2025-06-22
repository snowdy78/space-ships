#pragma once

#include "SoundDisperseEntity.hpp"
#include "AbstractManager.hpp"

class SoundManager : public AbstractManager<std::vector, SoundDisperseEntity>
{
public:
    SoundManager() = default;

    bool need_erase(const const_iterator &value) const override;
    void update_item(const const_iterator &value) override;
    void onPushValue(const const_iterator &value) override;
};
