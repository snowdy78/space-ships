#pragma once

#include <list>
#include "components/AnimatedSprite.hpp"
#include "decl.hpp"

template<class T>
class LoadFunction
{
public:
	static void load(const sf::String &, sf::Texture &) = delete;
};

template<>
class LoadFunction<sf::Texture>
{
public:
	static void load(const sf::String &path, sf::Texture &loading_value);
};
template<>
class LoadFunction<sf::SoundBuffer>
{
public:
	static void load(const sf::String &path, sf::SoundBuffer &loading_value);
};
template<>
class LoadFunction<sf::Font>
{
public:
	static void load(const sf::String &path, sf::Font &loading_value);
};
template<>
class LoadFunction<AnimatedSprite>
{
public:
	static void load(const sf::String &path, AnimatedSprite &sprite);
};

struct LoadingContentBase
{
	LoadingContentBase() = default;
	LoadingContentBase(const sf::String &path)
		: m_path(path)
	{
	}
	virtual void load() noexcept  = 0;
	virtual bool isLoad() const	  = 0;
	virtual ~LoadingContentBase() = 0;
	const sf::String &path() const
	{
		return m_path;
	}
	void path(const sf::String &_path)
	{
		m_path = _path;
	}
	/**
	 * @return a name of file
	 * example: a path `foo/bar/file.png` be `file.png`
	 *
	 */
	sf::String filename(const sf::String &delims = "/\\") const;
	/**
	 * @return a name of file without a file extension
	 * example: a path `foo/bar/file.png` be `file`
	 *
	 */
	sf::String stem() const;
	/**
	 * @return a file extension
	 */
	sf::String extension() const;

private:
	sf::String m_path;
};
class Loader
{
protected:
	Loader() = default;

public:
	using handle_load_function_type = std::function<void(LoadingContentBase &)>;
	virtual ~Loader()				= default;
	virtual void load(
		const handle_load_function_type &before_every_load = [](LoadingContentBase &) {},
		const handle_load_function_type &after_every_load  = [](LoadingContentBase &) {}
	)							= 0;
	virtual size_t size() const = 0;
	virtual bool isLoad() const = 0;
	virtual void loadBefore()
	{
	}
};
class LoaderContainer
{
	using loaders_type = std::list<Loader *>;
	using value_type   = loaders_type::value_type;
	template<LoaderConcept T>
	friend class FileLoader;
	static loaders_type &loaders();

public:
	static size_t encountLoads();
	static bool isAllLoad();
	static void loadAll(
		const std::function<void(LoadingContentBase &)> &before_load,
		const std::function<void(LoadingContentBase &)> &after_load
	);
};

template<LoaderConcept T>
class FileLoader : public Loader
{
	friend class Loader;

public:
	template<class U>
	using container_type = std::vector<U>;
	template<class U>
	using pointer_type = std::unique_ptr<U>;
	struct LoadingContent : LoadingContentBase
	{
		LoadingContent();
		LoadingContent(const sf::String &);
		LoadingContent(const LoadingContent &) = delete;
		LoadingContent(LoadingContent &&other) noexcept;
		~LoadingContent() override;
		void load() noexcept override;

		bool isLoad() const override;
		loading<T> get() const;
		loading_ptr<T> ptr() const;
		LoadingContent &operator=(LoadingContent &&other) noexcept;

	private:
		std::unique_ptr<T *> m_item;
	};
	using before_load_function_type = std::function<void(LoadingContentBase &)>;
	using after_load_function_type	= std::function<void(LoadingContentBase &)>;

	using value_type		  = pointer_type<LoadingContent>;
	using data_container_type = container_type<value_type>;

	static FileLoader &instance();

	void load(
		const handle_load_function_type &before_every_load = [](LoadingContentBase &) {},
		const handle_load_function_type &after_every_load  = [](LoadingContentBase &) {}
	) override;

	size_t size() const override;
	bool isLoad() const override;

	LoadingContent &addToUpload(const char *path);
	FileLoader(const FileLoader &)				  = delete;
	FileLoader(FileLoader &&) noexcept			  = delete;
	FileLoader &operator=(const FileLoader &)	  = delete;
	FileLoader &operator=(FileLoader &&) noexcept = delete;

private:
	data_container_type m_data;
	bool is_load = false;
	void clear();
	FileLoader();
};

using TextureLoader	  = FileLoader<sf::Texture>;
using SoundLoader	  = FileLoader<sf::SoundBuffer>;
using FontLoader	  = FileLoader<sf::Font>;
using AnimationLoader = FileLoader<AnimatedSprite>;

///////////////////////////
//
// template implementation
//

template<LoaderConcept T>
FileLoader<T>::LoadingContent::LoadingContent()
	: m_item(std::make_unique<T *>(nullptr))
{
}

template<LoaderConcept T>
FileLoader<T>::LoadingContent::LoadingContent(const sf::String &path)
	: LoadingContentBase(path),
	  m_item(std::make_unique<T *>(nullptr))
{
}

template<LoaderConcept T>
FileLoader<T>::LoadingContent::LoadingContent(LoadingContent &&other) noexcept: LoadingContentBase(std::move(other)),
	m_item(std::move(other))
{
	*other.m_item = nullptr;
}

template<LoaderConcept T>
FileLoader<T>::LoadingContent::~LoadingContent()
{
	if (m_item)
		delete *m_item;
}

template<LoaderConcept T>
void FileLoader<T>::LoadingContent::load() noexcept
{
	if (m_item && *m_item)
	{
#ifdef SPACE_SHIP_DEBUG
		std::cout << "repeated load: " << path().toAnsiString() << "\n";
#endif
		delete *m_item;
	}
	*m_item = new T;
	LoadFunction<T>::load(path(), **m_item);
}

template<LoaderConcept T>
bool FileLoader<T>::LoadingContent::isLoad() const
{
	return m_item && *m_item;
}

template<LoaderConcept T>
loading<T> FileLoader<T>::LoadingContent::get() const
{
	return *m_item;
}

template<LoaderConcept T>
loading_ptr<T> FileLoader<T>::LoadingContent::ptr() const
{
	return m_item.get();
}

template<LoaderConcept T>
typename FileLoader<T>::LoadingContent & FileLoader<T>::LoadingContent::operator=(LoadingContent &&other) noexcept
{
	if (this != &other)
	{
		static_cast<LoadingContentBase &>(*this) = std::move(other);
		m_item									 = std::move(other.m_item);
		other.m_item							 = nullptr;
	}
	return *this;
}

template<LoaderConcept T>
typename FileLoader<T>::LoadingContent &FileLoader<T>::addToUpload(const char *path)
{
	m_data.emplace_back(new LoadingContent(path));
	return *m_data.back();
}

template<LoaderConcept T>
void FileLoader<T>::clear()
{
	m_data.clear();
}
template<LoaderConcept T>
FileLoader<T> &FileLoader<T>::instance()
{
	// согласно стандарту, этот код ленивый и потокобезопасный
	static bool created = false;
	static FileLoader s;
	if (!created)
	{
		LoaderContainer::loaders().push_back(&s);
		created = true;
	}
	return s;
}

template<LoaderConcept T>
bool FileLoader<T>::isLoad() const
{
	return is_load || !std::ranges::any_of(m_data, [](const value_type &value) {
		return !value->isLoad();
	});
}

template<LoaderConcept T>
void FileLoader<T>::load(
	const before_load_function_type &before_every_load, const after_load_function_type &after_every_load
)
{
	for (auto &content: m_data)
	{
		before_every_load(*content);
		content->load();
		after_every_load(*content);
	}
	is_load = true;
}
template<LoaderConcept T>
size_t FileLoader<T>::size() const
{
	return m_data.size();
}

template<LoaderConcept T>
FileLoader<T>::FileLoader() = default;
