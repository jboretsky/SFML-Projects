#include "../Header/Entity.hpp"

Entity::Entity()
: mVelocity() {}

void Entity::setVelocity(sf::Vector2f velocity) {
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy) {
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const {
	return mVelocity;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands) {
	move(mVelocity * dt.asSeconds());
}

void Entity::destroy()
{

}