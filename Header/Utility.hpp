#ifndef UTILITY
#define UTILITY

#include <sstream>

namespace sf {
	class Sprite;
	class Text;
}

template <typename T>
std::string toString(const T& value);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

#endif