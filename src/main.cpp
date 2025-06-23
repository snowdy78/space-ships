#include <thread>

#include "Font.hpp"
#include "Gameplay.hpp"
#include "MainMenu.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"
#include "components/GameConfiguration.hpp"


void loadGame(sf::RenderWindow &window);

int main()
{
	auto resolution = rn::VideoSettings::getResolution();
	sf::RenderWindow window{
		{ resolution.x, resolution.y },
		"window"
	};

	window.setFramerateLimit(rn::VideoSettings::getFramerateLimit());
	window.setVerticalSyncEnabled(rn::VideoSettings::getVsync());
	window.setKeyRepeatEnabled(false);

	loadGame(window);

	// TODO: ServerTestBranch is temporary
#ifdef SPACE_SHIP_DEBUG
	try
	{
		rn::MenuBranch::start_branch<MainMenu>(window);
	}
	catch (std::exception &err)
	{
		std::cout << "dropped with an exception at: ";
		std::cout << err.what() << "\n";
		std::cin.get();
	}
#else
	rn::MenuBranch::start_branch<MainMenu>(window);
#endif
	return 0;
}
void loadGame(sf::RenderWindow &window)
{
	config::preload();

	sf::Color background_color{ sf::Color::Black };
	sf::Color progress_bar_outline_color{ sf::Color::White };
	sf::Color progress_bar_progress_color{ sf::Color::White };
	sf::Color progress_bar_background_color{ sf::Color::Transparent };
	sf::Color progress_pointer_color{ sf::Color::White };
	sf::Color text_color{ sf::Color::White };

	FileLoader<sf::Font>::instance().load();
	size_t count		= LoaderContainer::encountLoads();
	size_t count_loaded = 0;
	rn::Vec2f screen_size{ rn::VideoSettings::getResolution() };
	sf::Text load_content_path;
	sf::Text percentage;
	rn::Rect progress_bar{
		{ screen_size.x / 2.f, 12.f }
	};
	rn::Convex progress_pointer{ 3 };
	rn::Rect background{ screen_size };
	rn::Rect progress{ progress_bar.getSize() };

	background.setFillColor(background_color);

	progress_bar.setFillColor(progress_bar_background_color);
	progress_bar.setOutlineColor(progress_bar_outline_color);
	progress_bar.setOutlineThickness(1.f);
	progress_bar.setPosition((screen_size.x - progress_bar.getSize().x) / 2.f, screen_size.y / 2.f);

	load_content_path.setFont(*Font::Default);
	load_content_path.setCharacterSize(static_cast<unsigned>(screen_size.y) / 54u);
	load_content_path.setFillColor(text_color);
	load_content_path.setString(count == 0 ? "No files to load" : "preparing...");
	load_content_path.setPosition(
		floor((screen_size.x - load_content_path.getGlobalBounds().width) / 2.f),
		floor(progress_bar.getPosition().y - static_cast<float>(load_content_path.getCharacterSize()) - 10)
	);

	progress.setFillColor(progress_bar_progress_color);
	progress.setPosition((screen_size.x - progress_bar.getSize().x) / 2.f, screen_size.y / 2.f);

	progress_pointer.setFillColor(progress_pointer_color);
	progress_pointer.setPoint(0, { 10, 0 });
	progress_pointer.setPoint(1, { 20, 20 });
	progress_pointer.setPoint(2, { 0, 20 });
	progress_pointer.setPosition(
		progress.getPosition().x + progress.getSize().x - progress_pointer.getGlobalBounds().width / 2.f,
		progress.getPosition().y + progress.getSize().y
	);

	percentage.setFillColor(text_color);
	percentage.setFont(*Font::Default);
	percentage.setCharacterSize(screen_size.y / 45);
	percentage.setString(
		count == 0 ? "100%"
				   : std::to_string(static_cast<size_t>(
						 std::floor(static_cast<double>(count_loaded) / static_cast<double>(count * 100))
					 )) + "%"
	);
	percentage.setPosition(
		progress_pointer.getPosition().x, progress_pointer.getPosition().y + percentage.getGlobalBounds().height
	);


	bool is_load	= false;
	auto beforeLoad = [&]() {
		load_content_path.setPosition(
			static_cast<int>((screen_size.x - load_content_path.getGlobalBounds().width) / 2.f),
			load_content_path.getPosition().y
		);
	};
	auto afterLoad = [&]() {
		count_loaded++;

		progress.setSize({ count == 0 ? progress_bar.getSize().x
									  : progress_bar.getSize().x * static_cast<float>(count_loaded) / count,
						   progress.getSize().y });

		progress_pointer.setPosition(
			progress.getPosition().x + progress.getSize().x - progress_pointer.getGlobalBounds().width / 2.f,
			progress_pointer.getPosition().y
		);
		percentage.setString(
			count == 0 ? "100%"
					   : std::to_string(static_cast<size_t>(static_cast<float>(count_loaded) / count * 100)) + "%"
		);
		percentage.setOrigin(static_cast<int>(percentage.getGlobalBounds().width / 2.f), percentage.getOrigin().y);
		percentage.setPosition(
			static_cast<int>(progress_pointer.getPosition().x), static_cast<int>(percentage.getPosition().y)
		);
	};
	std::thread thread([&]() {
		LoaderContainer::loadAll(
			[&](const LoadingContentBase &preload_data) {
				mutex.lock();
				load_content_path.setString("loading path: " + preload_data.path());
				beforeLoad();
				mutex.unlock();
			},
			[&](LoadingContentBase &) {
				mutex.lock();
				afterLoad();
				mutex.unlock();
			}
		);
		mutex.lock();
		is_load = true;
		mutex.unlock();
	});
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		if (is_load)
			break;
		mutex.lock();
		window.clear();
		window.draw(background);
		window.draw(progress_bar);
		window.draw(progress);
		window.draw(progress_pointer);
		window.draw(percentage);
		window.draw(load_content_path);
		window.display();
		mutex.unlock();
	}
	thread.join();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
