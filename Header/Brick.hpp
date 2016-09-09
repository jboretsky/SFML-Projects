#ifndef BRICK
#define BRICK

#include "./Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Brick : public Entity {
	public:
		enum brickLayer {
			None,
			One,
			Two,
			Three,
			Unbreakable,
		};

	public:
		Brick(brickLayer layer, sf::Texture& texture);
		virtual unsigned int getCategory() const;

		virtual sf::FloatRect getBoundingRect() const;
		sf::IntRect getTypeCoords(brickLayer layer);

		void hit();
		void destroy();
		static Brick::brickLayer getType(int i);
		Brick::brickLayer getType() const;
		virtual bool isMarkedForRemoval() const;

	private:
		virtual bool isDestroyed() const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time dt);

	private:
		sf::Sprite mSprite;
		brickLayer mBrickLayer;
		bool mIsMarkedForRemoval;
};

#endif