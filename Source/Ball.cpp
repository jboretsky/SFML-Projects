#include "../Header/Ball.hpp"
#include "../Header/Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Ball::Ball(sf::Texture& texture, sf::IntRect textureRect)
: mSprite(texture, textureRect) {
	centerOrigin(mSprite);
}

unsigned int Ball::getCategory() const {
	return Category::Ball;
}

// void Ball::updateCurrent(sf::Time dt) {

// }

void Ball::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

sf::FloatRect Ball::getBoundingRect() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}