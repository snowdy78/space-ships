#include "Font.hpp"
#include "components/FileLoader.hpp"

loading<sf::Font> Font::Default	 = FontLoader::instance().addToUpload("C:/Windows/Fonts/arial.ttf").get();
loading<sf::Font> Font::Jersey10 = FontLoader::instance().addToUpload("fonts/Jersey10-Regular.ttf").get();
