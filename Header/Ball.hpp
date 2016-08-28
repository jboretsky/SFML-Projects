#ifndef BALL
#define BALL

#include "./Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Ball : public Entity {
	public:
		Ball(sf::Texture& texture, sf::IntRect textureRect);
		virtual unsigned int getCategory() const;

		virtual sf::FloatRect getBoundingRect() const;

	private:
		// virtual void updateCurrent(sf::Time dt);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Sprite mSprite;
};

#endif