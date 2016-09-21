#ifndef TILE
#define TILE

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Tile : public sf::Drawable {
	public:
		enum Type {
			Wall,
			Ground,
		};

	public:
		Tile(Type type, sf::Vector2f position);
		sf::Vector2f getPosition() const;
		sf::FloatRect getBounds() const;
		void setType(Type type);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

		void setColor();

	private:
		Type mType;
		sf::RectangleShape mTile;
};

#endif