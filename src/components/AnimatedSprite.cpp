#include "components/AnimatedSprite.hpp"
#include <algorithm>
#include <filesystem>
#include <stdexcept>


AnimationDataInfo::AnimationDataInfo(const sf::String &path_to_data_folder, const std::filesystem::path &file_extention)
	: path_to_folder(path_to_data_folder),
	  file_extention(file_extention)
{}

AnimatedSprite::Keyframe::Keyframe(const time_digit_t &duration, sf::Texture *const &texture)
	: m_duration(duration),
	  m_texture(texture)
{}

void AnimatedSprite::Keyframe::setTexture(sf::Texture *texture)
{
	m_texture.reset(texture);
}
void AnimatedSprite::Keyframe::setDuration(const time_digit_t &duration)
{
	m_duration = duration;
}
const sf::Texture &AnimatedSprite::Keyframe::getTexture() const
{
	return *m_texture;
}
const AnimatedSprite::time_digit_t &AnimatedSprite::Keyframe::getDuration() const
{
	return m_duration;
}

AnimatedSprite::AnimatedSprite(const time_digit_t &duration)
	: m_duration(duration)
{}

AnimatedSprite::AnimatedSprite(const AnimatedSprite &anim)
	: Effect(anim),
	  m_repeating(anim.getRepeating()),
	  m_clock(anim.m_clock),
	  m_duration(anim.m_duration)
{
	long long cur_kf_index = 0;	
	for (long long kf_index = 0; kf_index < anim.m_keyframes.size(); ++kf_index)
	{
		auto ik = anim.m_keyframes.begin() + kf_index;
		m_keyframes.emplace_back(new PrivKeyframe(**ik));
		if (ik == anim.m_current_keyframe) // finding current keyframe in a new array
		{
			cur_kf_index = kf_index;
		}
	}
	m_current_keyframe = m_keyframes.begin() + cur_kf_index;
}

bool AnimatedSprite::load(const AnimationDataInfo &datainfo)
{
	std::filesystem::directory_iterator di{ datainfo.path_to_folder.toAnsiString() };
	std::vector<std::string> paths{};
	for (auto &item: di)
	{
		if (item.is_regular_file())
		{
			std::wstring str{ item.path() };
			std::string path{ str.begin(), str.end() };
			if (str.find(datainfo.file_extention.c_str()))
			{
				paths.push_back(std::move(path));
			}
		}
	}
	time_digit_t average_keyframe_time{
		static_cast<size_t>(std::floor(static_cast<double>(m_duration.count()) / static_cast<double>(paths.size())))
	};
	for (auto &path: paths)
	{
		sf::Texture *texture{ new sf::Texture() };
		if (!texture->loadFromFile(path))
		{
			std::cerr << "texture not load, path is not correct: '" << path << "'\n";
			delete texture;
			m_keyframes.clear();
			return false;
		}
		std::unique_ptr<PrivKeyframe> keyframe{ new PrivKeyframe(average_keyframe_time, texture) };
		m_keyframes.push_back(std::move(keyframe));
	}
	if (getKeyframeCount())
	{
		setCurrentKeyframe(0);
	}
	return true;
}

void AnimatedSprite::start()
{
	m_current_keyframe = begin();
	setTexture(getKeyframe(0).getTexture());
	m_clock.reset();
	m_clock.start();
}

void AnimatedSprite::update()
{
	if (!m_clock.is_stopped() && m_clock.time<time_digit_t>().count() > (*m_current_keyframe)->getDuration().count())
	{
		++m_current_keyframe;
		if (m_current_keyframe == end())
		{
			if (m_repeating)
				start();
			else
				m_clock.stop();
		}
		else
			setTexture((*m_current_keyframe)->getTexture());
		m_clock.reset();
	}
}

bool AnimatedSprite::played() const
{
	return m_clock.is_stopped() && !m_keyframes.empty() && m_current_keyframe == end() && !m_repeating;
}

AnimatedSprite::const_iterator AnimatedSprite::begin() const
{
	return m_keyframes.begin();
}

AnimatedSprite::const_iterator AnimatedSprite::cbegin() const
{
	return m_keyframes.cbegin();
}

AnimatedSprite::const_iterator AnimatedSprite::end() const
{
	return m_keyframes.end();
}

AnimatedSprite::const_iterator AnimatedSprite::cend() const
{
	return m_keyframes.cend();
}

AnimatedSprite::const_reverse_iterator AnimatedSprite::rbegin() const
{
	return m_keyframes.rbegin();
}

AnimatedSprite::const_reverse_iterator AnimatedSprite::rend() const
{
	return m_keyframes.rend();
}

AnimatedSprite::iterator AnimatedSprite::begin()
{
	return m_keyframes.begin();
}

AnimatedSprite::iterator AnimatedSprite::end()
{
	return m_keyframes.end();
}

const AnimatedSprite::Keyframe &AnimatedSprite::getCurrentKeyframe() const
{
	return **m_current_keyframe;
}

const AnimatedSprite::Keyframe &AnimatedSprite::getKeyframe(size_t index) const
{
	return *m_keyframes.at(index);
}

AnimatedSprite::const_iterator AnimatedSprite::find(const Keyframe &keyframe) const
{
	auto it = std::find_if(m_keyframes.begin(), m_keyframes.end(), [&keyframe](auto &rightside_operand) {
		return &keyframe == &*rightside_operand;
	});
	if (it != m_keyframes.end())
		return it;
	return end();
}

void AnimatedSprite::setKeyframeTexture(size_t index, sf::Texture *texture)
{
	auto &keyframe = m_keyframes.at(index);
	keyframe->setTexture(texture);
}

void AnimatedSprite::setKeyframeDuration(size_t index, const time_digit_t &new_duration)
{
	auto &keyframe = m_keyframes.at(index);
	m_duration -= keyframe->getDuration();
	keyframe->setDuration(new_duration);
	m_duration += new_duration;
}

size_t AnimatedSprite::getKeyframeCount() const
{
	return m_keyframes.size();
}

void AnimatedSprite::setRepeating(bool repeating)
{
	m_repeating = repeating;
}

AnimatedSprite &AnimatedSprite::operator=(const AnimatedSprite &anim) 
{
	if (this != &anim)
	{
		m_repeating = anim.m_repeating;
		m_clock = anim.m_clock;
		m_duration = anim.m_duration;
		m_keyframes.clear();
		long long cur_kf_index = 0;	
		for (long long kf_index = 0; kf_index < anim.m_keyframes.size(); ++kf_index)
		{
			auto ik = anim.m_keyframes.begin() + kf_index;
			m_keyframes.emplace_back(new PrivKeyframe(**ik));
			if (ik == anim.m_current_keyframe) // finding current keyframe in a new array
			{
				cur_kf_index = kf_index;
			}
		}
		m_current_keyframe = m_keyframes.begin() + cur_kf_index;
	}
	return *this;
}

const AnimatedSprite::time_digit_t &AnimatedSprite::getDuration() const
{
	return m_duration;
}

bool AnimatedSprite::getRepeating() const
{
	return m_repeating;
}

void AnimatedSprite::setCurrentKeyframe(size_t index)
{
	if (index > getKeyframeCount())
		throw std::out_of_range("invalid keyframe index");
	if (index == getKeyframeCount())
		m_clock.stop();
	m_current_keyframe = begin() + static_cast<long long>(index);
	if (index != getKeyframeCount())
	{
		setTexture(m_keyframes.at(index)->getTexture());
	}
}
AnimatedSprite::Keyframe::Keyframe(const Keyframe &keyframe)
	: m_duration(keyframe.m_duration),
	  m_texture(new sf::Texture(*keyframe.m_texture))
{}
AnimatedSprite::Keyframe &AnimatedSprite::Keyframe::operator=(const Keyframe &keyframe)
{
	if (this != &keyframe)
	{
		m_duration = keyframe.m_duration;
		m_texture.reset(new sf::Texture(*keyframe.m_texture));
	}
	return *this;
}
