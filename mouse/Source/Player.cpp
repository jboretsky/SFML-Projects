#include "../Header/Player.hpp"
#include "../Header/Utility.hpp"

Player::Player()
: mVelocity(0,0)
, mPlayer(10) {
	centerOrigin(mPlayer);
	mPlayer.setFillColor(sf::Color::Black);
}

void Player::setVelocity(sf::Vector2f velocity) {
	mVelocity = velocity;
}

void Player::setVelocity(float x, float y) {
	mVelocity = sf::Vector2f(x,y);
}

// this function will need to be updated to cover pathfinding
void Player::update(sf::Time dt) {
	float vel = 300.f;
	mVelocity = sf::Vector2f(0.f,0.f);
	if(!mMoveQueue.empty()) {
		sf::Vector2f moveTo = mMoveQueue.back();
		sf::Vector2f playerPosition = getPosition();

		float yDist = moveTo.y - playerPosition.y;
		float xDist = moveTo.x - playerPosition.x;
		float angle = std::atan2(yDist, xDist);
		float error = 3.f;
		if (std::abs(playerPosition.x - moveTo.x) > error || std::abs(playerPosition.y - moveTo.y) > error) {
			setVelocity(vel * std::cos(angle), vel * std::sin(angle));
		} else {
			setPosition(moveTo);
			mMoveQueue.pop_back();
		}
		move(mVelocity.x * dt.asSeconds(), mVelocity.y * dt.asSeconds());
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
	mMoveQueue.push_front(tilePosition);
}