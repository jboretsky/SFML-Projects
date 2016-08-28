#include "../Header/Brick.hpp"
#include "../Header/Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

Brick::Brick(brickLayer layer, sf::Texture& texture)
: mBrickLayer(layer)
, mSprite(texture, getTypeCoords(layer)) {
	centerOrigin(mSprite);
}

unsigned int Brick::getCategory() const {
	return Category::Brick;
}

void Brick::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

void Brick::updateCurrent(sf::Time dt) {
	if (mBrickLayer <= 0) {
		// mIsMarkedForRemoval = true;
		return;
	}
}

sf::FloatRect Brick::getBoundingRect() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Brick::hit() {
	if (mBrickLayer == Three) {
		mBrickLayer = Two;
	} else if (mBrickLayer == Two) {
		mBrickLayer = One;
	} else if (mBrickLayer == One) {
		setPosition(-100,0);
	}
	mSprite.setTextureRect(getTypeCoords(mBrickLayer));
}

sf::IntRect Brick::getTypeCoords(brickLayer layer) {
	sf::IntRect rect;
	switch(layer) {
		case One:
			rect = sf::IntRect(0,0,66,20);
			break;
		case Two:
			rect = sf::IntRect(0,20,66,20);
			break;
		case Three:
			rect = sf::IntRect(0,40,66,20);
			break;
	}
	return rect;
}