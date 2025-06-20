#pragma once

#include "AbstractShip.hpp"
#include "coop/TransferableObject.hpp"


class ConnectedPlayerShip final : public AbstractShip, public TransferObjectBase<ConnectedPlayerShip>
{
	inline static loading<sf::Texture> texture = TextureLoader::instance().addToUpload("img/friend_ship.png").get();

public:
	ConnectedPlayerShip();
    void movement() override;
	void rotation() override;
    void onEvent(sf::Event &event) override;
	rn::Vec2f countMove() const override;
	void summonCopy(SpaceField &field) const override;
	void receiveJson(const rn::Json &json) override;
};
