#pragma once

#include "decl.hpp"

class KeyIcon : public rn::MonoBehaviour
{
    using scancode_e = sf::Keyboard::Scancode;
public:
    using map_key_texture = std::map<scancode_e, loading_ptr<sf::Texture>>;
    using map_key_char = std::map<scancode_e, char>;
private:
    static map_key_char key_map;
    
    static loading<sf::Texture> key_sample_texture;

    static sf::Color color;
    static map_key_texture key_textures;
    scancode_e m_key;
    sf::Sprite m_sprite;
    std::unique_ptr<sf::Text> m_key_text;
public:
    KeyIcon(scancode_e key);
	bool isIntersected(const rn::Vec2f &point) const override;
	scancode_e getScancode() const;
    ~KeyIcon() override = default;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

};
