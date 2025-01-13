#include "KeyIcon.hpp"
#include "Font.hpp"


KeyIcon::map_key_char KeyIcon::key_map = {
	{ scancode_e::A,				 'A'	 },
	{ scancode_e::B,				 'B'	 },
	{ scancode_e::C,				 'C'	 },
	{ scancode_e::D,				 'D'	 },
	{ scancode_e::E,				 'E'	 },
	{ scancode_e::F,				 'F'	 },
	{ scancode_e::G,				 'G'	 },
	{ scancode_e::H,				 'H'	 },
	{ scancode_e::I,				 'I'	 },
	{ scancode_e::J,				 'J'	 },
	{ scancode_e::K,				 'K'	 },
	{ scancode_e::L,				 'L'	 },
	{ scancode_e::M,				 'M'	 },
	{ scancode_e::N,				 'N'	 },
	{ scancode_e::O,				 'O'	 },
	{ scancode_e::P,				 'P'	 },
	{ scancode_e::Q,				 'Q'	 },
	{ scancode_e::R,				 'R'	 },
	{ scancode_e::S,				 'S'	 },
	{ scancode_e::T,				 'T'	 },
	{ scancode_e::U,				 'U'	 },
	{ scancode_e::V,				 'V'	 },
	{ scancode_e::W,				 'W'	 },
	{ scancode_e::X,				 'X'	 },
	{ scancode_e::Y,				 'Y'	 },
	{ scancode_e::Z,				 'Z'	 },
	{ scancode_e::Comma,			 ','	 },
	{ scancode_e::Slash,			 '?'	 },
	{ scancode_e::Grave,			 '`'	 },
	{ scancode_e::Equal,			 '='	 },
	{ scancode_e::Num0,			'0'	},
	{ scancode_e::Num1,			'1'	},
	{ scancode_e::Num2,			'2'	},
	{ scancode_e::Num3,			'3'	},
	{ scancode_e::Num4,			'4'	},
	{ scancode_e::Num5,			'5'	},
	{ scancode_e::Num6,			'6'	},
	{ scancode_e::Num7,			'7'	},
	{ scancode_e::Num8,			'8'	},
	{ scancode_e::Num9,			'9'	},
	{ scancode_e::NumpadDecimal,	 '.'	 },
	{ scancode_e::Period,		  '.'  },
	{ scancode_e::Hyphen,		  '-'  },
	{ scancode_e::LBracket,		'['	},
	{ scancode_e::RBracket,		']'	},
	{ scancode_e::Apostrophe,	  '\'' },
	{ scancode_e::NumpadDivide,	'/'	},
	{ scancode_e::NumpadMultiply, '*'  },
	{ scancode_e::NumpadMinus,	   '-'  },
	{ scancode_e::NumpadPlus,	  '+'  },
	{ scancode_e::Semicolon,		 ';'	 },
	{ scancode_e::Backslash,		 '\\' }
};

sf::Color KeyIcon::color{ 68, 0, 255 };

loading<sf::Texture> KeyIcon::key_sample_texture
	= FileLoader::Instance().addTextureToUpload("img/icons/0key-icon.png").get();
KeyIcon::map_key_texture KeyIcon::key_textures = {
	{ scancode_e::Space,		 FileLoader::Instance().addTextureToUpload("img/icons/space-key.png").ptr()		},
	{ scancode_e::Enter,		 FileLoader::Instance().addTextureToUpload("img/icons/enter-key.png").ptr()		},
	{ scancode_e::Escape,	  FileLoader::Instance().addTextureToUpload("img/icons/escape-key.png").ptr()		  },
	{ scancode_e::Backspace,	 FileLoader::Instance().addTextureToUpload("img/icons/backspace-key.png").ptr()	},
	{ scancode_e::Left,		FileLoader::Instance().addTextureToUpload("img/icons/arrow-left-key.png").ptr()	},
	{ scancode_e::Right,		 FileLoader::Instance().addTextureToUpload("img/icons/arrow-right-key.png").ptr()  },
	{ scancode_e::Up,		  FileLoader::Instance().addTextureToUpload("img/icons/arrow-up-key.png").ptr()		},
	{ scancode_e::Down,		FileLoader::Instance().addTextureToUpload("img/icons/arrow-down-key.png").ptr()	},
	{ scancode_e::Tab,		   FileLoader::Instance().addTextureToUpload("img/icons/tab-key.png").ptr()			},
	{ scancode_e::LControl,	FileLoader::Instance().addTextureToUpload("img/icons/lctrl-key.png").ptr()		   },
	{ scancode_e::RControl,	FileLoader::Instance().addTextureToUpload("img/icons/rctrl-key.png").ptr()		   },
	{ scancode_e::LShift,	  FileLoader::Instance().addTextureToUpload("img/icons/lshift-key.png").ptr()		  },
	{ scancode_e::RShift,	  FileLoader::Instance().addTextureToUpload("img/icons/rshift-key.png").ptr()		  },
	{ scancode_e::LAlt,		FileLoader::Instance().addTextureToUpload("img/icons/lalt-key.png").ptr()		  },
	{ scancode_e::RAlt,		FileLoader::Instance().addTextureToUpload("img/icons/ralt-key.png").ptr()		  },
	{ scancode_e::Menu,		FileLoader::Instance().addTextureToUpload("img/icons/menu-key.png").ptr()		  },
	{ scancode_e::Pause,		 FileLoader::Instance().addTextureToUpload("img/icons/pause-key.png").ptr()		},
	{ scancode_e::Insert,	  FileLoader::Instance().addTextureToUpload("img/icons/insert-key.png").ptr()		  },
	{ scancode_e::Delete,	  FileLoader::Instance().addTextureToUpload("img/icons/delete-key.png").ptr()		  },
	{ scancode_e::PageUp,	  FileLoader::Instance().addTextureToUpload("img/icons/pageup-key.png").ptr()		  },
	{ scancode_e::PageDown,	FileLoader::Instance().addTextureToUpload("img/icons/pagedown-key.png").ptr()	  },
	{ scancode_e::Home,		FileLoader::Instance().addTextureToUpload("img/icons/home-key.png").ptr()		  },
	{ scancode_e::End,		   FileLoader::Instance().addTextureToUpload("img/icons/end-key.png").ptr()			},
	{ scancode_e::Numpad0,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad0-key.png").ptr()		},
	{ scancode_e::Numpad1,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad1-key.png").ptr()		},
	{ scancode_e::Numpad2,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad2-key.png").ptr()		},
	{ scancode_e::Numpad3,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad3-key.png").ptr()		},
	{ scancode_e::Numpad4,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad4-key.png").ptr()		},
	{ scancode_e::Numpad5,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad5-key.png").ptr()		},
	{ scancode_e::Numpad6,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad6-key.png").ptr()		},
	{ scancode_e::Numpad7,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad7-key.png").ptr()		},
	{ scancode_e::Numpad8,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad8-key.png").ptr()		},
	{ scancode_e::Numpad9,	   FileLoader::Instance().addTextureToUpload("img/icons/numpad9-key.png").ptr()		},
	{ scancode_e::F1,		  FileLoader::Instance().addTextureToUpload("img/icons/f1-key.png").ptr()			  },
	{ scancode_e::F2,		  FileLoader::Instance().addTextureToUpload("img/icons/f2-key.png").ptr()			  },
	{ scancode_e::F3,		  FileLoader::Instance().addTextureToUpload("img/icons/f3-key.png").ptr()			  },
	{ scancode_e::F4,		  FileLoader::Instance().addTextureToUpload("img/icons/f4-key.png").ptr()			  },
	{ scancode_e::F5,		  FileLoader::Instance().addTextureToUpload("img/icons/f5-key.png").ptr()			  },
	{ scancode_e::F6,		  FileLoader::Instance().addTextureToUpload("img/icons/f6-key.png").ptr()			  },
	{ scancode_e::F7,		  FileLoader::Instance().addTextureToUpload("img/icons/f7-key.png").ptr()			  },
	{ scancode_e::F8,		  FileLoader::Instance().addTextureToUpload("img/icons/f8-key.png").ptr()			  },
	{ scancode_e::F9,		  FileLoader::Instance().addTextureToUpload("img/icons/f9-key.png").ptr()			  },
	{ scancode_e::F10,		   FileLoader::Instance().addTextureToUpload("img/icons/f10-key.png").ptr()			},
	{ scancode_e::F11,		   FileLoader::Instance().addTextureToUpload("img/icons/f11-key.png").ptr()			},
	{ scancode_e::F12,		   FileLoader::Instance().addTextureToUpload("img/icons/f12-key.png").ptr()			},
	{ scancode_e::CapsLock,	FileLoader::Instance().addTextureToUpload("img/icons/capslock-key.png").ptr()	  },
	{ scancode_e::NumLock,	   FileLoader::Instance().addTextureToUpload("img/icons/numlock-key.png").ptr()		},
	{ scancode_e::ScrollLock,  FileLoader::Instance().addTextureToUpload("img/icons/scrolllock-key.png").ptr()	  },
	{ scancode_e::PrintScreen, FileLoader::Instance().addTextureToUpload("img/icons/printscreen-key.png").ptr()	},
	{ scancode_e::NumpadEnter, FileLoader::Instance().addTextureToUpload("img/icons/numpad-enter-key.png").ptr() },
};

KeyIcon::KeyIcon(scancode_e key)
	: key_text(nullptr)
{
	if (key_map.find(key) != key_map.end())
	{
		key_text = new sf::Text(sf::String(key_map[key]), *Font::Jersey10);
		sprite.setTexture(*key_sample_texture);
	}
	else if (key_textures.find(key) != key_textures.end())
	{
		sprite.setTexture(**key_textures.at(key));
	}
}
KeyIcon::KeyIcon(const KeyIcon &other)
	: sprite(other.sprite),
	  key(other.key)
{
	key_text = other.key_text ? new sf::Text(*other.key_text) : nullptr;
}
KeyIcon::KeyIcon(KeyIcon &&other)
	: sprite(std::move(other.sprite)),
	  key(std::move(other.key))
{
	key_text = other.key_text ? new sf::Text(*other.key_text) : nullptr;
	delete other.key_text;
	other.key_text = nullptr;
}
KeyIcon::~KeyIcon()
{
	delete key_text;
}

KeyIcon &KeyIcon::operator=(const KeyIcon &other)
{
	if (this != &other)
	{
		sprite = other.sprite;
		key	   = other.key;
		delete key_text;
		key_text = other.key_text ? new sf::Text(*other.key_text) : nullptr;
	}
	return *this;
}
KeyIcon &KeyIcon::operator=(KeyIcon &&other) noexcept
{
	if (this != &other)
	{
		sprite = std::move(other.sprite);
		key	   = std::move(other.key);
		delete key_text;
		key_text = other.key_text;
		delete other.key_text;
		other.key_text = nullptr;
	}
	return *this;
}
bool KeyIcon::isIntersected(const rn::Vec2f &point) const
{
	return sprite.getGlobalBounds().contains(point);
}
void KeyIcon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
	if (key_text)
		target.draw(*key_text, states);
}
