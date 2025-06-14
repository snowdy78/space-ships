#pragma once

#include "Setting.hpp"

struct AudioSettings : Setting
{
protected:
	AudioSettings()
		: Setting(byDefault())
	{
		if (checkKeyContainRecursively(json_file.json, byDefault()))
			json_file.save();
		m_general = json_file.json["audio"]["general_volume"];
		m_music	  = json_file.json["audio"]["music_volume"];
		m_sfx	  = json_file.json["audio"]["sfx_volume"];
	}

public:
	static json_type byDefault()
	{
		return {
			{ "audio",
			  { { "general_volume", default_general_volume },
				{ "music_volume", default_music_volume },
				{ "sfx_volume", default_sfx_volume } } }
		};
	}

	GET_SETTING_FUNC_IMPL(getGeneralVolume, m_general)
	GET_SETTING_FUNC_IMPL(getMusicVolume, m_music)
	GET_SETTING_FUNC_IMPL(getSFXVolume, m_sfx)
	SET_SETTING_FUNC_IMPL(setGeneralVolume, "general_volume")
	SET_SETTING_FUNC_IMPL(setMusicVolume, "music_volume")
	SET_SETTING_FUNC_IMPL(setSFXVolume, "sfx_volume")

private:
	constexpr static float default_general_volume = 0.7f;
	constexpr static float default_music_volume	  = 1.0f;
	constexpr static float default_sfx_volume	  = 1.0f;
	float m_general								  = default_general_volume;
	float m_music								  = default_music_volume;
	float m_sfx									  = default_sfx_volume;
};

inline float general_volume_percentage()
{
	return AudioSettings::getGeneralVolume() * 100;
}
inline float music_volume_percentage()
{
	return general_volume_percentage() * AudioSettings::getMusicVolume();
}
inline float sfx_volume_percentage()
{
	return general_volume_percentage() * AudioSettings::getSFXVolume();
}
