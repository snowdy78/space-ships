#include "components/GameInfo.hpp"
#include "components/GameInfoData.hpp"

rn::Vec2f GameInfo::getSampleSize()
{
	rn::Vec2f res{ rn::VideoSettings::getResolution() };
	return { res.x / 2.f, static_cast<float>(GameInfoData::character_size) };
}

size_t GameInfo::getRowCount() 
{ 
	rn::Vec2f res{ rn::VideoSettings::getResolution() };
	return static_cast<size_t>(res.y / getSampleSize().y);
}

GameInfo::GameInfo()
	: table(2, getRowCount(), getSampleSize())
{
	data.reserve(20);
}

void GameInfo::addData(sf::String name, std::function<sf::String()> get_data)
{
	data.emplace_back(name, get_data);
	size_t y = (data.size() - 1) % table.getRowCount();
	size_t x = (data.size() - 1) / table.getRowCount();
	data[data.size() - 1].setPosition(table.getCellGlobalPos(x, y));
}

void GameInfo::setVisible(bool visible) { this->visible = visible; }

bool GameInfo::isVisible() const { return visible; }

void GameInfo::removeData(size_t index)
{
	auto it = data.begin();
    if (it >= data.end())
        throw std::out_of_range("index out of range");    

    std::advance(it, index);
    data.erase(it);
}

void GameInfo::removeData(const sf::String &name) 
{
    auto it = std::find_if(data.begin(), data.end(), [&](GameInfoData &data) {
        return data.getName() == name;
    });
    if (it != data.end())
        data.erase(it);
}

void GameInfo::updateData(size_t index) 
{
    data[index].updateData();
}

void GameInfo::updateData(const sf::String &name) 
{
    auto it = std::find_if(data.begin(), data.end(), [&](GameInfoData &data) {
        return data.getName() == name;
    });
    if (it == data.end())
        throw std::out_of_range("index out of range");
    it->updateData();
}

void GameInfo::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!visible)
        return;
	states.transform *= getTransform();
	for (auto &value : data)
	{
		target.draw(value, states);
	}
}
