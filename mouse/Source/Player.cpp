#include "../Header/Player.hpp"
#include "../Header/Utility.hpp"

Player::Player()
: mVelocity(0,0)
, mPlayer(10) {
	centerOrigin(mPlayer);
}

void Player::setVelocity(sf::Vector2f velocity) {
	mVelocity = velocity;
}

void Player::setVelocity(float x, float y) {
	mVelocity = sf::Vector2f(x,y);
}

void Player::update() {
	move(mVelocity);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mPlayer, states);
}

sf::FloatRect Player::getBounds() const {
	return mPlayer.getGlobalBounds();
}