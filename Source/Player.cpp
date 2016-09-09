#include "../Header/Player.hpp"
#include "../Header/CommandQueue.hpp"
#include "../Header/Paddle.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

struct PaddleMover {
	PaddleMover(float vx, float vy)
	: velocity(vx, vy)
	{}

	void operator() (Paddle& paddle, sf::Time) const {
		paddle.setVelocity(velocity);
	}
	sf::Vector2f velocity;
};

Player::Player() 
: mLives(3) {
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;

	initializeActions();

	for (auto &pair: mActionBindings) {
		pair.second.category = Category::Player;
	}
}

// if keypressed event occurs, and the pressed key appears in
// the key bindings, push the command to the queue
void Player::handleEvent(const sf::Event& event, CommandQueue& commands) {
	if(event.type == sf::Event::KeyPressed) {
		auto found = mKeyBinding.find(event.key.code);
		if(found != mKeyBinding.end() && !isRealtimeAction(found->second)){
			commands.push(mActionBindings[found->second]);
		}
	}
}

// traverse all the assigned keys and check if any are pressed
// if so, look up the action and push the command to the queue
void Player::handleRealtimeInput(CommandQueue& commands) {
	for (auto &pair: mKeyBinding) {
		if(sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second)) {
			commands.push(mActionBindings[pair.second]);
		}
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key) {
	for (auto itr= mKeyBinding.begin(); itr != mKeyBinding.end();) {
		if (itr->second == action) mKeyBinding.erase(itr++);
		else ++itr;
	}
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const {
	for (auto pair: mKeyBinding) {
		if (pair.second == action) return pair.first;
	}
	return sf::Keyboard::Unknown;
}

void Player::initializeActions() {
	const float playerSpeed = 400.f;

	mActionBindings[MoveLeft].action = derivedAction<Paddle>(PaddleMover(-playerSpeed, 0.f));
	mActionBindings[MoveRight].action = derivedAction<Paddle>(PaddleMover(+playerSpeed, 0.f));
}

bool Player::isRealtimeAction(Action action) {
	switch(action) {
		case MoveLeft:
		case MoveRight:
			return true;
		default:
			return false;
	}
}

int Player::getLives() const {
	return mLives;
}