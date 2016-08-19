#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Player : public sf::Drawable {
	public:
		Player();
		void draw(sf::RenderTarget& target, sf::RenderStates states);

	private:
		sf::Sprite mSprite;
};

#endif