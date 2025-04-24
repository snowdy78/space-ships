#include "components/FileLoader.hpp"

FileLoader &FileLoader::Instance()
{
	// согласно стандарту, этот код ленивый и потокобезопасный
	static FileLoader s;
	return s;
}

void FileLoader::loadTextures(
	std::function<void(LoadingTexture &)> before_every_load, std::function<void(LoadingTexture &)> after_every_load
)
{
	loadContent(textures, before_every_load, after_every_load);
}

void FileLoader::loadFonts(
	std::function<void(LoadingFont &)> before_every_load, std::function<void(LoadingFont &)> after_every_load
)
{
	loadContent(fonts, before_every_load, after_every_load);
}

void FileLoader::loadSounds(
	std::function<void(LoadingSound &)> before_every_load, std::function<void(LoadingSound &)> after_every_load
)
{
	loadContent(sound_buffers, before_every_load, after_every_load);
}

size_t FileLoader::getTextureCount() const
{
	return textures.size();
}

size_t FileLoader::getSoundCount() const
{
	return sound_buffers.size();
}

size_t FileLoader::getFontCount() const
{
	return fonts.size();
}

const FileLoader::LoadingSound &FileLoader::addSoundToUpload(const char *path)
{
	return addToUpload(sound_buffers, path);
}

const FileLoader::LoadingFont &FileLoader::addFontToUpload(const char *path)
{
	return addToUpload(fonts, path);
}

const FileLoader::LoadingTexture &FileLoader::addTextureToUpload(const char *path)
{
	return addToUpload(textures, path);
}

void FileLoader::clearSoundLoadingContent()
{
	clearLoadingContent(sound_buffers);
}

void FileLoader::clearFontLoadingContent()
{
	clearLoadingContent(fonts);
}

void FileLoader::clearTextureLoadingContent()
{
	clearLoadingContent(textures);
}

template<class T>
const FileLoader::LoadingContent<T> &FileLoader::addToUpload(std::vector<LoadingContent<T> *> &upload_container, const char *path)
{
	upload_container.emplace_back(new LoadingContent<T>(path, [](const sf::String &path, T &content) {
		if (!content.loadFromFile(path))
		{
			throw std::out_of_range("File not found: '" + path + "'");
		}
	}));
	return *upload_container.back();
}

template<class T>
void FileLoader::loadContent(
		std::vector<LoadingContent<T> *> &upload_container,
		std::function<void(LoadingContent<T> &)> before_every_load,
		std::function<void(LoadingContent<T> &)> after_every_load)
{
	for (auto &content: upload_container)
	{
		before_every_load(*content);
		content->load();
		after_every_load(*content);
	}
}

template<class T>
void FileLoader::clearLoadingContent(std::vector<LoadingContent<T> *> &upload_container)
{
	for (auto &content : upload_container)
	{
		delete content;
	}
	upload_container.clear();
}

FileLoader::FileLoader() 
{
}

FileLoader::~FileLoader() 
{
	clearSoundLoadingContent();
	clearFontLoadingContent();
	clearTextureLoadingContent();
}
