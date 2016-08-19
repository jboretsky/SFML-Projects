#ifndef RESOURCEIDENTIFIERS
#define RESOURCEIDENTIFIERS

#include <SFML/Graphics.hpp>

namespace Textures
{
	enum ID
	{
		Paddle,
		WidePaddle,
		GunPaddle,
		BackgroundImage,
		TitleScreen,
	};
}

namespace Fonts {
	enum ID {
		Main,
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

// FontHolder and TextureHolder are typedefs for 
//our own ResourceHolder class template
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif