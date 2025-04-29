#include "components/AnimatedSprite.hpp"
#include <filesystem>
#include <stdexcept>

AnimationDataInfo::AnimationDataInfo(
	const sf::String &path_to_data_folder, const sf::String &mime_file_type,
	const sf::String &file_indexing_regex,
	const sf::String &keyframes_sort)
: path_to_folder(path_to_data_folder),
  mime_file_type(mime_file_type),
  file_indexing_regex(file_indexing_regex),
  keyframes_sort(keyframes_sort)
{

}

AnimatedSprite::Keyframe::Keyframe(const time_digit_t &duration, sf::Texture *const &texture)
	: m_duration(duration),
	  m_texture(texture)
{}

void AnimatedSprite::Keyframe::setTexture(sf::Texture *texture)
{
	m_texture = texture;
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

bool AnimatedSprite::load(const AnimationDataInfo &datainfo)
{
	std::filesystem::directory_iterator di{ datainfo.path_to_folder.toAnsiString() };
	if (!di->is_directory())
	{
		std::cerr << "path should reference to directory";
		return false;
	}
	for (auto &item : di)
	{
		std::cout << item.path() << "\n";
	}
	return true;
}

void AnimatedSprite::update() 
{
	if (m_clock.getElapsedTime().asMilliseconds() > m_current_keyframe->getDuration().count())
	{
		++m_current_keyframe;
		m_sprite.setTexture(m_current_keyframe->getTexture());
	}
	if (m_current_keyframe == end() && m_repeating)
		m_current_keyframe = begin();
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
	return *m_current_keyframe;
}

const AnimatedSprite::Keyframe &AnimatedSprite::getKeyframe(size_t index) const 
{
	return m_keyframes.at(index);
}

AnimatedSprite::const_iterator AnimatedSprite::find(const Keyframe &keyframe) const 
{
	auto it = std::find_if(m_keyframes.begin(), m_keyframes.end(), 
		[&keyframe](const Keyframe &rightside_operand) 
		{
			return &keyframe == &rightside_operand; 
		}
	);
	if (it != m_keyframes.end())
		return it;
	return end();
}

void AnimatedSprite::setKeyframeTexture(size_t index, sf::Texture *texture) 
{
	auto &keyframe = m_keyframes.at(index);
	keyframe.setTexture(texture);
}

void AnimatedSprite::setKeyframeDuration(size_t index, const time_digit_t &new_duration) 
{
	auto &keyframe = m_keyframes.at(index);
	m_duration -= keyframe.getDuration();
	keyframe.setDuration(new_duration);
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

void AnimatedSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const 
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}

const AnimatedSprite::time_digit_t &AnimatedSprite::getDuration() const 
{
	return m_duration;
}

bool AnimatedSprite::getRepeating() const
{
	return m_repeating;
}
