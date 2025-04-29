#pragma once

#include <chrono>
#include "decl.hpp"
#include "Effect.hpp"

struct AnimationDataInfo
{
	AnimationDataInfo(
		const sf::String &path_to_data_folder, const sf::String &mime_file_type
	);

	const sf::String path_to_folder;
	const sf::String mime_file_type;
};

/**
 * @brief displays sprite that uses animation with keyframes.
 */
class AnimatedSprite : public Effect
{
	class Keyframe;
public:
private:
	using keyframe_vector = std::vector<std::unique_ptr<Keyframe>>;
	
	class Keyframe
	{
		friend class AnimatedSprite;
		Keyframe(const time_digit_t &duration, sf::Texture *const &texture);
	public:
		Keyframe(const Keyframe &) = delete;
		Keyframe(Keyframe &&) noexcept = delete;
		const time_digit_t &getDuration() const;
		const sf::Texture &getTexture() const;
		Keyframe &operator=(const Keyframe &) = delete;
		Keyframe &operator=(Keyframe &&) noexcept = delete;
	private:
		void setDuration(const time_digit_t &duration);
		void setTexture(sf::Texture *texture);

	private:
		time_digit_t m_duration;
		std::unique_ptr<sf::Texture> m_texture;
	};

public:
	using iterator				 = keyframe_vector::iterator;
	using const_iterator		 = keyframe_vector::const_iterator;
	using reverse_iterator		 = keyframe_vector::reverse_iterator;
	using const_reverse_iterator = keyframe_vector::const_reverse_iterator;

	AnimatedSprite() = default;
	AnimatedSprite(const time_digit_t &duration);

	bool load(const AnimationDataInfo &datainfo);
	void setDuration(const time_digit_t &duration);
	void start() override;
	void update() override;
	bool played() const override;

	const_iterator begin() const;
	const_iterator cbegin() const;
	const_iterator end() const;
	const_iterator cend() const;
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

private:
	iterator begin();
	iterator end();

public:
	const Keyframe &getCurrentKeyframe() const;
	const Keyframe &getKeyframe(size_t index) const;
	const_iterator find(const Keyframe &keyframe) const;
	size_t getKeyframeCount() const;
	const time_digit_t &getDuration() const;
	bool getRepeating() const;

	void setCurrentKeyframe(size_t index);
	void setKeyframeTexture(size_t index, sf::Texture *);
	void setKeyframeDuration(size_t index, const time_digit_t &new_duration);
	/**
	 * @brief removes all keyframes in range `start + 1` before `stop` and increase keyframe duration at `start` index
	 * on sum of removed durations of keyframes
	 *
	 * @param start
	 * @param stop - last keyframe which not includes into remove range
	 */
	template<class IterT>
	void spanKeyframes(IterT start, const IterT &stop);
	void setRepeating(bool repeating);

private:
	bool m_repeating = false;
	sf::Clock m_clock;
	keyframe_vector m_keyframes{};
	time_digit_t m_duration{ 0 };
	iterator m_current_keyframe{ end() };
};

template<class IterT>
void AnimatedSprite::spanKeyframes(IterT start, const IterT &stop) 
{
	if (start == m_keyframes.end())
		throw std::out_of_range("keyframe is out of range");
	time_digit_t add_duration{0};
	auto &keyframe = *start;
	if (++start == m_keyframes.end())
		throw std::out_of_range("keyframe is out of range");
	for (; start != stop; ++start)
	{
		add_duration += start->getDuration();
		m_keyframes.erase(start);
	}
	keyframe->duration += add_duration;
}