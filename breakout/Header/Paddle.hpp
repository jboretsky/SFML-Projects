#ifndef PADDLE
#define PADDLE

#include "./Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Paddle : public Entity {
	public:
		Paddle(const sf::Texture& texture);
		Paddle(const sf::Texture& texture, const sf::IntRect& textureRect);

		virtual sf::FloatRect getBoundingRect() const;
		virtual unsigned int getCategory() const;

		void enlarge();
	private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Sprite mSprite;
};

#endif