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
	float vel = 3.f;
	mVelocity = sf::Vector2f(0.f,0.f);
	if(!mMoveQueue.empty()) {
		sf::Vector2f moveTo = mMoveQueue.back();
		sf::Vector2f playerPosition = getPosition();

		float yDist = moveTo.y - playerPosition.y;
		float xDist = moveTo.x - playerPosition.x;
		float angle = std::atan2(yDist, xDist);
		float error = 2;
		if (std::abs(playerPosition.x - moveTo.x) > error || std::abs(playerPosition.y - moveTo.y) > error) {
			setVelocity(vel * std::cos(angle), vel * std::sin(angle));
			std::cout << std::abs(playerPosition.x - moveTo.x) << std::endl;
			std::cout << std::abs(playerPosition.y - moveTo.y) << std::endl;
		} else {
			setPosition(moveTo);
			mMoveQueue.pop_back();
		}
		move(mVelocity);
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mPlayer, states);
}

sf::FloatRect Player::getBounds() const {
	return mPlayer.getGlobalBounds();
}

void Player::moveTo(sf::Vector2f tilePosition) {
	mMoveQueue.insert(mMoveQueue.begin(),tilePosition);
}