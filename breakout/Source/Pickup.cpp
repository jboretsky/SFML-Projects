#include "../Header/Pickup.hpp"
#include "../Header/Category.hpp"
#include "../Header/CommandQueue.hpp"
#include "../Header/Utility.hpp"
#include "../Header/ResourceHolder.hpp"
#include "../Header/DataTables.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace {
	const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures)
: mType(type)
, mSprite(textures.get(Table[type].texture))
, mIsMarkedForRemoval(false) {
	centerOrigin(mSprite);
}


unsigned int Pickup::getCategory() const {
	return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const {
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Paddle& player) const {
	Table[mType].action(player);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

bool Pickup::isMarkedForRemoval() const {
	return mIsMarkedForRemoval;
}

void Pickup::destroy() {
	mIsMarkedForRemoval = true;
	std::cout << "Destroying" << std::endl;
}