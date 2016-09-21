#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <vector>
#include <iostream>

class Player: public sf::Drawable, public sf::Transformable {
	public:
		Player();
		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float x, float y);
		void update();
		sf::FloatRect getBounds() const;
		void moveTo(sf::Vector2f tilePosition);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::CircleShape mPlayer;
		std::vector<sf::Vector2f> mMoveQueue;

		sf::Vector2f mVelocity;

};

#endif