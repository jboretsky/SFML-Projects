#ifndef UTILITY
#define UTILITY

#include <sstream>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace sf {
	class Sprite;
	class Text;
}

template <typename T>
std::string toString(const T& value);
std::string toString(int i);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(sf::CircleShape& circle);
void centerOrigin(sf::RectangleShape& rect);

#endif