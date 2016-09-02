#include "../Header/Utility.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cmath>

template <typename T>
std::string toString(const T& value) {
	std::stringstream stream;
	stream << value;
	return stream.str();
}

std::string toString(int i) {
	std::stringstream stream;
	stream << i;
	return stream.str();
}

void centerOrigin(sf::Sprite& sprite) {
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text) {
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}