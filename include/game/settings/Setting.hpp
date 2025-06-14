#pragma once

#include "decl.hpp"

#ifndef SET_SETTING_FUNC_IMPL

#define SET_SETTING_FUNC_IMPL(func_name, key_string)                                   \
	static void func_name(float volume)                                                \
	{                                                                                  \
		AudioSettings settings;                                                        \
		settings.json_file.json["audio"][key_string] = std::clamp(volume, 0.0f, 1.0f); \
		settings.json_file.save();                                                     \
	}

#endif

#ifndef GET_SETTING_FUNC_IMPL

#define GET_SETTING_FUNC_IMPL(func_name, var_name) \
	static float func_name()                       \
	{                                              \
		return AudioSettings().var_name;           \
	}

#endif

struct Setting : rn::Setting
{
	using json_type	 = rn::Json;
	using jsonf_type = rn::JsonFile;

protected:
	using rn::Setting::Setting;
	static bool hasParameter(const json_type &json, const json_type::string_t &key)
	{
		return !json.contains(key);
	}

	/**
	 * @brief recursively finds the keys of a default json object in a given json and sets the default value from the
	 * corresponding default json if not found
	 * @param json
	 * @param default_json
	 * @returns true if some value was set as default
	 */
	static bool checkKeyContainRecursively(rn::Json &json, const rn::Json &default_json)
	{
		bool was_defaulted = false;
		for (auto &i: default_json.items())
		{
			if (!json.contains(i.key()))
			{
				json[i.key()] = default_json[i.key()];
				was_defaulted = true;
			}
			else if (i.value().is_object())
			{
				checkKeyContainRecursively(json[i.key()], default_json[i.key()]);
			}
		}
		return was_defaulted;
	}
};
