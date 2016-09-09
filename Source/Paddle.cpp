#include "../Header/Paddle.hpp"
#include "../Header/Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

Paddle::Paddle(const sf::Texture& texture)
: mSprite(texture) {
	centerOrigin(mSprite);
}

Paddle::Paddle(const sf::Texture& texture, const sf::IntRect& textureRect)
: mSprite(texture, textureRect) {
	centerOrigin(mSprite);
}

void Paddle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

unsigned int Paddle::getCategory() const {
	return Category::Player;
}

sf::FloatRect Paddle::getBoundingRect() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Paddle::enlarge() {
	mSprite.setTextureRect(sf::IntRect(0,20,120,20));
	centerOrigin(mSprite);
}