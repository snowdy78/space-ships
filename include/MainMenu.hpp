#pragma once

#include "RuneEngine/LogicalObject.hpp"
#include "components/Background.hpp"
#include "Font.hpp"
#include "components/FileLoader.hpp"
class MainMenu : public rn::MenuBranch
{

	class Button : public rn::Button, public rn::LogicalObject
	{
		inline static std::vector<Button *> buttons;
	public:
		std::function<void()> onclick;
		using rn::LogicalObject::update;
		Button(const sf::String &placeholder, const sf::Font &font = *Font::Jersey10)
			: rn::Button(getSize(), placeholder, font)
		{
			buttons.emplace_back(this);
			content.setFillColor(sf::Color::White);
			setFillColor(sf::Color{ 12, 76, 236 });
		}
		~Button() override
		{
			auto it = std::ranges::find(buttons, this);
			if (it != buttons.end())
				buttons.erase(it);
		}
		void start() override
		{
			static_cast<sf::Text &>(content).setPosition(rn::Vec2f{ rn::Vec2i{ content.getPosition() } });
		}
		void onEvent(sf::Event &event) override
		{
			if (isClicked(sf::Mouse::Left))
			{
				onclick();
			}
		}
		static const std::vector<Button *> &getButtons()
		{
			return buttons;
		}
		static rn::Vec2f getSize()
		{
			rn::Vec2f res{ rn::VideoSettings::getResolution() };
			return { res.x / 5, res.y / 10 };
		}
	};

public:
	using rn::MenuBranch::MenuBranch;
	~MainMenu() override;
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;

private:
	inline static loading<sf::Texture> logo{ TextureLoader::instance().addToUpload("img/icons/logo.png").get() };
	inline static loading<sf::SoundBuffer> sound_track_buff{
		SoundLoader::instance().addToUpload("assets/darkfall.wav").get()
	};

	rn::Vec2f res{ rn::VideoSettings::getResolution() };
	Button play_button{ "Play" };
#ifdef SPACE_SHIP_DEBUG
	Button host_button{ "Host" };
	Button connect_button{ "Connect" };
#endif
	Button exit_button{ "Exit" };

	sf::Sound sound_track{ *sound_track_buff };
	Background background{};
	sf::Sprite logo_sprite{ *logo };
};
