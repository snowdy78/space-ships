#pragma once

#include "decl.hpp"
#include "coop/ClientSocket.hpp"
#include <optional>
#include "coop/TransferableObject.hpp"


class Character : public TransferableObject, public rn::Circle, public rn::ClickableObject
{
	static size_t id;
	std::string name;

public:
	Character() {}
	void setName(const std::string &string)
	{
		name = string;
	}
	const std::string &getName() const
	{
		return name;
	}
	void receiveJson(const rn::Json &json) override
	{
		std::cout << "setting json: " << json.dump() << "\n";
		setRadius(json.at("radius"));
		setPosition(json.at("position").at("x"), json.at("position").at("y"));
		setName(json.at("name"));
	}
	bool isIntersected(const rn::Vec2f &point) const override
	{
		return rn::math::ellipce(*this).contains(point);
	}
	TransferJson toJson() const override
	{
		return {
			id,
			{ 
			{ "name", name },
			{ "radius", getRadius() },
			{
				"position",
				{ { "x", getPosition().x }, { "y", getPosition().y } },
			} }
		};
	}
};


class ServerTestBranch : public rn::MenuBranch
{
    std::optional<ClientSocket> client;
    rn::Button p1{{}, "P1", *Font::Default};
    rn::Button p2{{}, "P2", *Font::Default};
    rn::Button send_button{{}, "Send", *Font::Default};
    sf::Text send_status{"", *Font::Default};
    sf::Text receive_status{"", *Font::Default};
    Character character;
public:
    ServerTestBranch(sf::RenderWindow &window) : rn::MenuBranch(window) {}
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
};
