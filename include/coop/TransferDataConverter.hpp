#pragma once

#include "TransferableObject.hpp"
#include "decl.hpp"

template<class T>
concept CanBeConvertedConcept = std::is_base_of_v<TransferObjectBase<T>, T>;

struct ConvertFunction
{
	rn::Json operator()(const rn::Json &json) const
	{
		return json;
	}
};

class TransferDataConverter
{
	explicit TransferDataConverter() = default;

public:
	using convert_function_t = std::function<rn::Json(const rn::Json &)>;
	struct key_type
	{
		const size_t &identifier;
		key_type(const size_t &identifier)
			: identifier(identifier)
		{
		}
		auto operator<=>(const key_type &other) const
		{
			if (identifier < other.identifier)
				return std::strong_ordering::less;
			if (identifier > other.identifier)
				return std::strong_ordering::greater;
			return std::strong_ordering::equal;
		}
	};
	struct ConvertType
	{
		const size_t &identifier;
		convert_function_t convert;
		ConvertType(const size_t &id, convert_function_t convert)
			: identifier(id),
			  convert(std::move(convert))
		{
		}
	};

	using value_type = ConvertType;
	template<class K, class Val>
	using container = std::map<K, Val>;
	using conversions_t = container<key_type, value_type>;
	using const_iterator = conversions_t::const_iterator;
	static TransferDataConverter &instance()
	{
		static TransferDataConverter converter;
		return converter;
	}
	const_iterator begin() const
	{
		return m_conversions.begin();
	}
	const_iterator end() const
	{
		return m_conversions.end();
	}
	template<CanBeConvertedConcept FromType, CanBeConvertedConcept ToType>
	const value_type &push(convert_function_t conversion)
	{
		m_conversions.insert(
			{
				key_type(FromType::identifier),
				value_type{ ToType::identifier, conversion }
		   }
		);
		std::cout << "created conversion from " << typeid(FromType).name() << " to " << typeid(ToType).name()
				  << " (id: from " << FromType::identifier << " to " << ToType::identifier << ")\n";
		return m_conversions.at(key_type(FromType::identifier));
	}
	template<CanBeConvertedConcept FromType>
	bool contains() const
	{
		return this->contains(key_type(FromType::identifier));
	}
	bool contains(const key_type &id) const
	{
		return m_conversions.contains(id);
	}
	template<CanBeConvertedConcept FromType>
	std::optional<value_type> find() const
	{
		return this->find(FromType::identifier);
	}
	std::optional<value_type> find(size_t from_type_id) const
	{
		auto it = m_conversions.find(from_type_id);
		if (it != m_conversions.end())
			return it->second;
		return std::nullopt;
	}

private:
	conversions_t m_conversions{};
};

template<class FromType, class ToType, class ConvertFunc>
class Converted : public TransferObjectBase<FromType>
{
	static const TransferDataConverter::value_type &createConversion();
	static const TransferDataConverter::value_type &conversion()
	{
		static const TransferDataConverter::value_type &conv = createConversion();
		return conv;
	}
	const TransferDataConverter::value_type &_;

public:
	Converted()
		: _(conversion())
	{
	}
	~Converted() override;

	rn::Json convert() const
	{
		return conversion().convert(this->toJson());
	}
};

template<class FromType, class ToType, class ConvertFunc>
const TransferDataConverter::value_type &Converted<FromType, ToType, ConvertFunc>::createConversion()
{
	return TransferDataConverter::instance().push<FromType, ToType>(
		TransferDataConverter::convert_function_t([](const rn::Json &json) {
			return ConvertFunc()(json);
		})
	);
}

template<class FromType, class ToType, class ConvertFunc>
Converted<FromType, ToType, ConvertFunc>::~Converted() = default;
