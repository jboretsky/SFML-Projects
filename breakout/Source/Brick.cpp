#include "../Header/Brick.hpp"
#include "../Header/Utility.hpp"
#include "../Header/CommandQueue.hpp"
#include "../Header/DataTables.hpp"
#include "../Header/ResourceHolder.hpp"
#include "../Header/Pickup.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

Brick::Brick(brickLayer layer, TextureHolder& textures)
: mBrickLayer(layer)
, mSprite(textures.get(Textures::Bricks), getTypeCoords(layer))
, mDropPickupCommand()
, mIsMarkedForRemoval(false) {
	centerOrigin(mSprite);

	mDropPickupCommand.category = Category::Scene;
	mDropPickupCommand.action = [this, &textures] (SceneNode& node, sf::Time) {
		createPickup(node, textures);
	};
}

unsigned int Brick::getCategory() const {
	if (mBrickLayer != Brick::Unbreakable) return Category::Brick;
	return Category::Unbreakable;
}

void Brick::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

void Brick::updateCurrent(sf::Time dt, CommandQueue& commands) {
	if (isDestroyed()) {
		checkPickupDrop(commands);

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
		case Unbreakable:
			rect = sf::IntRect(0,60,66,20);
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
	} else if (i == 4) {
		return Brick::Unbreakable;
	}
}

Brick::brickLayer Brick::getType() const {
	return mBrickLayer;
}

void Brick::createPickup(SceneNode& node, const TextureHolder& textures) const {
	
	auto type = Pickup::Enlarge;

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 100.f);
	node.attachChild(std::move(pickup));
}

void Brick::checkPickupDrop(CommandQueue& commands) {
	int random = rand() % 4;
	if (random == 0) {
		commands.push(mDropPickupCommand);
	}
}

bool Brick::isDestroyed() const {
	return mBrickLayer == None;
}

void Brick::destroy() {
	mIsMarkedForRemoval = true;
}

bool Brick::isMarkedForRemoval() const {
	return mIsMarkedForRemoval;
}