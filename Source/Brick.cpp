#include "../Header/Brick.hpp"
#include "../Header/Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

Brick::Brick(brickLayer layer, sf::Texture& texture)
: mBrickLayer(layer)
, mSprite(texture, getTypeCoords(layer))
, mIsMarkedForRemoval(false) {
	centerOrigin(mSprite);
}

unsigned int Brick::getCategory() const {
	return Category::Brick;
}

void Brick::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

void Brick::updateCurrent(sf::Time dt) {
	if (mBrickLayer == None) {
		mIsMarkedForRemoval = true;
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
		mBrickLayer = None;
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

Brick::brickLayer Brick::getType(int i) {
	if (i == 1) {
		return Brick::One;
	} else if (i == 2) {
		return Brick::Two;
	} else if (i == 3) {
		return Brick::Three;
	}
}

bool Brick::isDestroyed() const {
	return mBrickLayer == Brick::None;
}

bool Brick::isMarkedForRemoval() const {
	return mIsMarkedForRemoval;
}