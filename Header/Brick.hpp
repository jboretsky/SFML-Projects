#ifndef BRICK
#define BRICK

#include "./Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Brick : public Entity {
	public:
		enum brickLayer {
			One = 1,
			Two = 2,
			Three = 3,
		};

	public:
		Brick(brickLayer layer, sf::Texture& texture);
		virtual unsigned int getCategory() const;

		virtual sf::FloatRect getBoundingRect() const;
		sf::IntRect getTypeCoords(brickLayer layer);

		void hit();

	private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time dt);

	private:
		sf::Sprite mSprite;
		brickLayer mBrickLayer;
};

#endif