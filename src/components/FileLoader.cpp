#include "components/FileLoader.hpp"

sf::String LoadingContentBase::filename(const sf::String &delims) const
{
	std::filesystem::path p(path());
	return std::filesystem::path(path()).filename().c_str();
}
sf::String LoadingContentBase::stem() const
{
	std::filesystem::path p(path());
	return std::filesystem::path(path()).stem().c_str();
}

sf::String LoadingContentBase::extension() const
{
	return std::filesystem::path(path()).extension().c_str();
}

LoaderContainer::loaders_type &LoaderContainer::loaders()
{
	static loaders_type ls;
	return ls;
}

size_t LoaderContainer::encountLoads()
{
	size_t total = 0;
	for (auto &loader: loaders())
		total += loader->size();
	return total;
}

bool LoaderContainer::isAllLoad()
{
	return !std::ranges::any_of(loaders(), [](value_type const &loader) {
		return !loader->isLoad();
	});
}

void LoaderContainer::loadAll(
	const std::function<void(LoadingContentBase &)> &before_load,
	const std::function<void(LoadingContentBase &)> &after_load
)
{
	for (auto &loader: loaders())
		if(!loader->isLoad())
			loader->load(before_load, after_load);
}

void LoadFunction<sf::Texture>::load(const sf::String &path, sf::Texture &loading_value)
{
	loading_value.setRepeated(true);
	if (!loading_value.loadFromFile(path))
		throw std::out_of_range("texture at path: '" + path + "' was not load!");
}

void LoadFunction<sf::SoundBuffer>::load(const sf::String &path, sf::SoundBuffer &loading_value)
{
	if (!loading_value.loadFromFile(path))
		throw std::out_of_range("sound at path: '" + path + "' was not load!");
}

void LoadFunction<sf::Font>::load(const sf::String &path, sf::Font &loading_value)
{
	if (!loading_value.loadFromFile(path))
		throw std::out_of_range("font at path: '" + path + "' was not load!");
}

void LoadFunction<AnimatedSprite>::load(const sf::String &path, AnimatedSprite &sprite)
{
	std::filesystem::path p{ path };
	auto t					= path.toAnsiString().find_last_of('.');
	auto ext				= path.substring(t).toAnsiString();
	std::string parent_path = p.parent_path().string();
	if (!sprite.load({ parent_path, ext }))
	{
		using namespace std::string_literals;
		throw std::out_of_range(
			"animation with path: '"s + parent_path + "' and extension '" + ext + "' was not load!"
		);
	}
}

LoadingContentBase::~LoadingContentBase() = default;
