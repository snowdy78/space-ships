#pragma once

#include "components/AnimatedSprite.hpp"

template<class T>
using loading = const T *const &;
template<class T>
using loading_ptr = const T *const *;

class FileLoader
{
public:
	template<class T>
	struct LoadingContent
	{
		using load_func_t = std::function<void(const sf::String &path, T &)>;

		LoadingContent();
		LoadingContent(const sf::String &path, load_func_t load_function);
		LoadingContent(const LoadingContent &other) = delete;
		LoadingContent(LoadingContent &&other) noexcept;
		~LoadingContent();
		void load();

		const sf::String &getLoadPath() const;
		bool isLoaded() const;
		loading<T> get() const;
		loading_ptr<T> ptr() const;
		sf::String base_name(const sf::String & delims = "/\\");
		sf::String name_noext();

		void setLoadPath(const sf::String &path);
		void setLoadFunction(load_func_t load_function);

		LoadingContent &operator=(const LoadingContent &other) = delete;
		LoadingContent &operator=(LoadingContent &&other) noexcept;

	private:
		T **texture				  = new T *(nullptr);
		sf::String path			  = "";
		load_func_t load_function = [](const sf::String &path, T &) {};
	};

	using LoadingTexture		= LoadingContent<sf::Texture>;
	using LoadingSound			= LoadingContent<sf::SoundBuffer>;
	using LoadingFont			= LoadingContent<sf::Font>;
	using LoadingAnimatedSprite = LoadingContent<AnimatedSprite>;

	static FileLoader &Instance();
	void loadTextures(
		std::function<void(LoadingTexture &)> before_every_load = [](LoadingTexture &) {},
		std::function<void(LoadingTexture &)> after_every_load	= [](LoadingTexture &) {}
	);
	void loadFonts(
		std::function<void(LoadingFont &)> before_every_load = [](LoadingFont &) {},
		std::function<void(LoadingFont &)> after_every_load	 = [](LoadingFont &) {}
	);
	void loadSounds(
		std::function<void(LoadingSound &)> before_every_load = [](LoadingSound &) {},
		std::function<void(LoadingSound &)> after_every_load  = [](LoadingSound &) {}
	);
	void loadAnimatedSprites(
		std::function<void(LoadingAnimatedSprite &)> before_every_load = [](LoadingAnimatedSprite &) {},
		std::function<void(LoadingAnimatedSprite &)> after_every_load  = [](LoadingAnimatedSprite &) {}
	);
	size_t getTextureCount() const;
	size_t getSoundCount() const;
	size_t getFontCount() const;
	size_t getAnimatedSpriteCount() const;
	const LoadingSound &addSoundToUpload(const char *path);
	const LoadingFont &addFontToUpload(const char *path);
	const LoadingTexture &addTextureToUpload(const char *path);
	const LoadingAnimatedSprite &addAnimatedSpriteToUpload(const char *path, const std::filesystem::path &file_extention);

private:
	std::vector<LoadingAnimatedSprite *> anim_sprites;
	std::vector<LoadingSound *> sound_buffers;
	std::vector<LoadingFont *> fonts;
	std::vector<LoadingTexture *> textures;

	template<class T>
	const LoadingContent<T> &addToUpload(
		std::vector<LoadingContent<T> *> &upload_container, const char *path,
		const LoadingContent<T>::load_func_t &load_function
	);

	template<class T>
	void loadContent(
		std::vector<LoadingContent<T> *> &upload_container,
		std::function<void(LoadingContent<T> &)> before_every_load = [](LoadingContent<T> &) {},
		std::function<void(LoadingContent<T> &)> after_every_load  = [](LoadingContent<T> &) {}
	);
	void clearSoundLoadingContent();
	void clearFontLoadingContent();
	void clearTextureLoadingContent();
	void clearAnimSpriteLoadingContent();

	template<class T>
	void clearLoadingContent(std::vector<LoadingContent<T> *> &upload_container);

	FileLoader();
	~FileLoader();

	FileLoader(FileLoader const &)			  = delete;
	FileLoader &operator=(FileLoader const &) = delete;
};
///////////////////////////
//
// template implementation
//
template<class T>
inline FileLoader::LoadingContent<T>::LoadingContent()
	: load_function([](const sf::String &, T &) {})
{}
template<class T>
inline FileLoader::LoadingContent<T>::LoadingContent(
	const sf::String &path, std::function<void(const sf::String &, T &)> load_function
)
	: texture(new T *(nullptr)),
	  path(path),
	  load_function(load_function)
{}

template<class T>
inline FileLoader::LoadingContent<T>::LoadingContent(LoadingContent &&other) noexcept
	: texture(other.texture),
	  path(other.path)
{
	*other.texture = nullptr;
	other.texture  = nullptr;
}

template<class T>
inline FileLoader::LoadingContent<T>::~LoadingContent()
{
	if (texture)
		delete *texture;
	delete texture;
}

template<class T>
inline void FileLoader::LoadingContent<T>::load()
{
	delete *texture;
	*texture = new T;
	load_function(path, **texture);
}

template<class T>
inline const sf::String &FileLoader::LoadingContent<T>::getLoadPath() const
{
	return path;
}

template<class T>
inline void FileLoader::LoadingContent<T>::setLoadFunction(std::function<void(const sf::String &, T &)> load_function)
{
	this->load_function = load_function;
}

template<class T>
inline bool FileLoader::LoadingContent<T>::isLoaded() const
{
	return texture;
}

template<class T>
inline const T *const &FileLoader::LoadingContent<T>::get() const
{
	return *texture;
}

template<class T>
inline const T *const *FileLoader::LoadingContent<T>::ptr() const
{
	return texture;
}

/**
 * @brief returns a name of file
 * example: a path `foo/bar/myfile.png` be `myfile.png`  
 * 
 */
template<class T>
inline sf::String FileLoader::LoadingContent<T>::base_name(const sf::String & delims)
{
	std::wstring wpath{path.begin(), path.end()};
	sf::String result{wpath.substr(wpath.find_last_of(delims) + 1)};
	return result;
}

/**
 * @brief returns a name of file
 * example: a path `foo/bar/myfile.png` be `myfile`  
 * 
 */
template<class T>
inline sf::String FileLoader::LoadingContent<T>::name_noext()
{
	std::wstring base_name{this->base_name()};
	size_t p(base_name.find_last_of('.'));
	return {p > 0 && p != sf::String::InvalidPos ? base_name.substr(0, p) : base_name};
}

template<class T>
inline void FileLoader::LoadingContent<T>::setLoadPath(const sf::String &path)
{
	this->path = path;
}

template<class T>
inline FileLoader::LoadingContent<T> &FileLoader::LoadingContent<T>::operator=(LoadingContent &&other) noexcept
{
	if (&other != this)
	{
		delete *texture;
		*texture	   = *other.texture;
		*other.texture = nullptr;
		delete other.texture;
		other.texture = nullptr;
	}
	return *this;
}
