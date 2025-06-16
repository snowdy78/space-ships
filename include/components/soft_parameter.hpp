#pragma once

#include <RuneEngine/Engine.hpp>

template<class T, class DurationT = std::chrono::milliseconds>
class soft_parameter : public rn::LogicalObject
{

public:
	using value_type	= T;
	using duration_type = DurationT;
	explicit soft_parameter();
	explicit soft_parameter(
		T initial, T max, const duration_type &time, const duration_type &start_delay = duration_type(0)
	);

	void start() override;
	bool strict() const;
	bool is_stopped() const;
	const T &max() const;
	const duration_type &time_limit() const;
	T value() const;
	operator float() const;
	auto operator<=>(const T &other) const;

private:
	rn::Stopwatch m_clock;
	duration_type m_time;
	T m_value;
	T m_max;
};

template<class T, class DurationT>
soft_parameter<T, DurationT>::soft_parameter()
	: m_time(duration_type(0)),
	  m_value(0),
	  m_max(0)
{
}

template<class T, class DurationT>
soft_parameter<T, DurationT>::soft_parameter(
	T initial, T max, const duration_type &time, const duration_type &start_delay
)
	: m_clock(-start_delay),
	  m_time(time),
	  m_value(initial),
	  m_max(max)
{
}

template<class T, class DurationT>
void soft_parameter<T, DurationT>::start()
{
	m_clock.start();
}

template<class T, class DurationT>
bool soft_parameter<T, DurationT>::strict() const
{
	using namespace std::chrono_literals;
	return m_clock.is_stopped() || !std::less<duration_type>()(m_clock.time<duration_type>(), time_limit());
}

template<class T, class DurationT>
bool soft_parameter<T, DurationT>::is_stopped() const
{
	return m_clock.is_stopped();
}

template<class T, class DurationT>
const T &soft_parameter<T, DurationT>::max() const
{
	return m_max;
}

template<class T, class DurationT>
const typename soft_parameter<T, DurationT>::duration_type &soft_parameter<T, DurationT>::time_limit() const
{
	return m_time;
}

template<class T, class DurationT>
T soft_parameter<T, DurationT>::value() const
{
	T t = static_cast<T>(std::max(0ll, m_clock.time<duration_type>().count()));
	T k = t / static_cast<T>(m_time.count());
	T p = std::lerp(m_value, m_max, k);
	return m_value < m_max ? std::min(p, m_max) : std::max(p, m_max);
}

template<class T, class DurationT>
soft_parameter<T, DurationT>::operator float() const
{
	return value();
}

template<class T, class DurationT>
auto soft_parameter<T, DurationT>::operator<=>(const T &other) const
{
	if (m_value < other)
		return std::strong_ordering::less;
	if (m_value > other)
		return std::strong_ordering::greater;
	return std::strong_ordering::equal;
}
