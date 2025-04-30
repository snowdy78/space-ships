#pragma once

#include <type_traits>
#include "Effect.hpp"
#include "components/AbstractManager.hpp"

struct EffectManager : public AbstractManager<std::vector, Effect>, public sf::Drawable
{
    EffectManager() = default;

    bool need_erase(const const_iterator &value) const override;
    void update_item(const const_iterator &value) override;
    void onPushValue(const const_iterator &value) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
