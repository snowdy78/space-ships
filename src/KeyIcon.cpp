#include "KeyIcon.hpp"
#include "Font.hpp"
#include "components/FileLoader.hpp"

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
	= TextureLoader::instance().addToUpload("img/icons/0key-icon.png").get();
KeyIcon::map_key_texture KeyIcon::key_textures = {
	{ scancode_e::Space,		 TextureLoader::instance().addToUpload("img/icons/space-key.png").ptr()		},
	{ scancode_e::Enter,		 TextureLoader::instance().addToUpload("img/icons/enter-key.png").ptr()		},
	{ scancode_e::Escape,	  TextureLoader::instance().addToUpload("img/icons/escape-key.png").ptr()		  },
	{ scancode_e::Backspace,	 TextureLoader::instance().addToUpload("img/icons/backspace-key.png").ptr()	},
	{ scancode_e::Left,		TextureLoader::instance().addToUpload("img/icons/arrow-left-key.png").ptr()	},
	{ scancode_e::Right,		 TextureLoader::instance().addToUpload("img/icons/arrow-right-key.png").ptr()  },
	{ scancode_e::Up,		  TextureLoader::instance().addToUpload("img/icons/arrow-up-key.png").ptr()		},
	{ scancode_e::Down,		TextureLoader::instance().addToUpload("img/icons/arrow-down-key.png").ptr()	},
	{ scancode_e::Tab,		   TextureLoader::instance().addToUpload("img/icons/tab-key.png").ptr()			},
	{ scancode_e::LControl,	TextureLoader::instance().addToUpload("img/icons/lctrl-key.png").ptr()		   },
	{ scancode_e::RControl,	TextureLoader::instance().addToUpload("img/icons/rctrl-key.png").ptr()		   },
	{ scancode_e::LShift,	  TextureLoader::instance().addToUpload("img/icons/lshift-key.png").ptr()		  },
	{ scancode_e::RShift,	  TextureLoader::instance().addToUpload("img/icons/rshift-key.png").ptr()		  },
	{ scancode_e::LAlt,		TextureLoader::instance().addToUpload("img/icons/lalt-key.png").ptr()		  },
	{ scancode_e::RAlt,		TextureLoader::instance().addToUpload("img/icons/ralt-key.png").ptr()		  },
	{ scancode_e::Menu,		TextureLoader::instance().addToUpload("img/icons/menu-key.png").ptr()		  },
	{ scancode_e::Pause,		 TextureLoader::instance().addToUpload("img/icons/pause-key.png").ptr()		},
	{ scancode_e::Insert,	  TextureLoader::instance().addToUpload("img/icons/insert-key.png").ptr()		  },
	{ scancode_e::Delete,	  TextureLoader::instance().addToUpload("img/icons/delete-key.png").ptr()		  },
	{ scancode_e::PageUp,	  TextureLoader::instance().addToUpload("img/icons/pageup-key.png").ptr()		  },
	{ scancode_e::PageDown,	TextureLoader::instance().addToUpload("img/icons/pagedown-key.png").ptr()	  },
	{ scancode_e::Home,		TextureLoader::instance().addToUpload("img/icons/home-key.png").ptr()		  },
	{ scancode_e::End,		   TextureLoader::instance().addToUpload("img/icons/end-key.png").ptr()			},
	{ scancode_e::Numpad0,	   TextureLoader::instance().addToUpload("img/icons/numpad0-key.png").ptr()		},
	{ scancode_e::Numpad1,	   TextureLoader::instance().addToUpload("img/icons/numpad1-key.png").ptr()		},
	{ scancode_e::Numpad2,	   TextureLoader::instance().addToUpload("img/icons/numpad2-key.png").ptr()		},
	{ scancode_e::Numpad3,	   TextureLoader::instance().addToUpload("img/icons/numpad3-key.png").ptr()		},
	{ scancode_e::Numpad4,	   TextureLoader::instance().addToUpload("img/icons/numpad4-key.png").ptr()		},
	{ scancode_e::Numpad5,	   TextureLoader::instance().addToUpload("img/icons/numpad5-key.png").ptr()		},
	{ scancode_e::Numpad6,	   TextureLoader::instance().addToUpload("img/icons/numpad6-key.png").ptr()		},
	{ scancode_e::Numpad7,	   TextureLoader::instance().addToUpload("img/icons/numpad7-key.png").ptr()		},
	{ scancode_e::Numpad8,	   TextureLoader::instance().addToUpload("img/icons/numpad8-key.png").ptr()		},
	{ scancode_e::Numpad9,	   TextureLoader::instance().addToUpload("img/icons/numpad9-key.png").ptr()		},
	{ scancode_e::F1,		  TextureLoader::instance().addToUpload("img/icons/f1-key.png").ptr()			  },
	{ scancode_e::F2,		  TextureLoader::instance().addToUpload("img/icons/f2-key.png").ptr()			  },
	{ scancode_e::F3,		  TextureLoader::instance().addToUpload("img/icons/f3-key.png").ptr()			  },
	{ scancode_e::F4,		  TextureLoader::instance().addToUpload("img/icons/f4-key.png").ptr()			  },
	{ scancode_e::F5,		  TextureLoader::instance().addToUpload("img/icons/f5-key.png").ptr()			  },
	{ scancode_e::F6,		  TextureLoader::instance().addToUpload("img/icons/f6-key.png").ptr()			  },
	{ scancode_e::F7,		  TextureLoader::instance().addToUpload("img/icons/f7-key.png").ptr()			  },
	{ scancode_e::F8,		  TextureLoader::instance().addToUpload("img/icons/f8-key.png").ptr()			  },
	{ scancode_e::F9,		  TextureLoader::instance().addToUpload("img/icons/f9-key.png").ptr()			  },
	{ scancode_e::F10,		   TextureLoader::instance().addToUpload("img/icons/f10-key.png").ptr()			},
	{ scancode_e::F11,		   TextureLoader::instance().addToUpload("img/icons/f11-key.png").ptr()			},
	{ scancode_e::F12,		   TextureLoader::instance().addToUpload("img/icons/f12-key.png").ptr()			},
	{ scancode_e::CapsLock,	TextureLoader::instance().addToUpload("img/icons/capslock-key.png").ptr()	  },
	{ scancode_e::NumLock,	   TextureLoader::instance().addToUpload("img/icons/numlock-key.png").ptr()		},
	{ scancode_e::ScrollLock,  TextureLoader::instance().addToUpload("img/icons/scrolllock-key.png").ptr()	  },
	{ scancode_e::PrintScreen, TextureLoader::instance().addToUpload("img/icons/printscreen-key.png").ptr()	},
	{ scancode_e::NumpadEnter, TextureLoader::instance().addToUpload("img/icons/numpad-enter-key.png").ptr() },
};

KeyIcon::KeyIcon(scancode_e key)
	: m_key(sf::Keyboard::Scancode::Unknown),
	  m_key_text(nullptr)
{
	if (key_map.contains(key))
	{
		m_key_text.reset(new sf::Text(sf::String(key_map[key]), *Font::Jersey10));
		m_sprite.setTexture(*key_sample_texture);
	}
	else if (key_textures.contains(key))
	{
		m_sprite.setTexture(**key_textures.at(key));
	}
}
bool KeyIcon::isIntersected(const rn::Vec2f &point) const
{
	return m_sprite.getGlobalBounds().contains(point);
}

KeyIcon::scancode_e KeyIcon::getScancode() const
{
	return m_key;
}

void KeyIcon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
	if (m_key_text)
		target.draw(*m_key_text, states);
}
