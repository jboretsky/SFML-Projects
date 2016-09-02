#include "../Header/World.hpp"
#include "../Header/ResourceIdentifiers.hpp"
#include "../Header/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <cmath>

World::World(sf::RenderWindow& window, FontHolder& fonts)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mLevelManager()
, mSceneGraph()
, mSceneLayers()
, mCommandQueue()
, mPlayerPaddle(nullptr)
, mLives(3)
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 1.05f) {

	mLevelManager.openFromFile("./media/levels.txt");
	loadTextures();
	buildScene();
}

void World::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::update(sf::Time dt) {
	mPlayerPaddle->setVelocity(0.f, 0.f);

	while(!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	} 

	mSceneGraph.update(dt);
	handleCollisions();

	mSceneGraph.removeWrecks();
	checkPosition();
	recalculateBallPosition();
}

void World::buildScene() {
	for(std::size_t i = 0; i < LayerCount; ++i) {
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}
	
	sf::IntRect textureRect(0,0,80,20);
	std::unique_ptr<Paddle> player(new Paddle(mTextures.get(Textures::Paddle), textureRect));
	mPlayerPaddle = player.get();
	mPlayerPaddle->setPosition(mSpawnPosition);
	mSceneLayers[Foreground]->attachChild(std::move(player));

	sf::IntRect ballRect(160, 200, 16, 16);
	std::unique_ptr<Ball> ball(new Ball(mTextures.get(Textures::Ball), ballRect));
	mBall = ball.get();
	mBall->setPosition(mPlayerPaddle->getWorldPosition().x, mPlayerPaddle->getWorldPosition().y - mPlayerPaddle->getBoundingRect().height / 2.f - mBall->getBoundingRect().height / 2.f);
	mBall->setVelocity(-150.f, -150.f);
	mSceneLayers[Foreground]->attachChild(std::move(ball));

	std::vector<LevelManager::BrickInfo*> currentLevelInfo = mLevelManager.getCurrentLevelVector();

	for (int i = 0; i < currentLevelInfo.size(); ++i) {
		std::unique_ptr<Brick> brick(new Brick(Brick::getType(currentLevelInfo[i]->type), mTextures.get(Textures::Bricks)));
		brick->setPosition(currentLevelInfo[i]->position);
		mSceneLayers[Foreground]->attachChild(std::move(brick));
	}

	// int coords = mWorldBounds.width / 66;
	// for (int j = 0; j < 3; ++j) {
	// 	for (int i = 0; i < coords; ++i) {
	// 		if (i != 0 && i != coords-1) {
	// 			std::unique_ptr<Brick> brick(new Brick(Brick::Three, mTextures.get(Textures::Bricks)));
	// 			brick->setPosition((float)(15 + 66*i + (brick->getBoundingRect().width / 2)), (float)(20 + 20*j + (brick->getBoundingRect().width / 2)));
	// 			mSceneLayers[Foreground]->attachChild(std::move(brick));
	// 		}
	// 	}
	// }
}

// std::vector<LevelManager::BrickInfo*> World::getNextLevel() {

// }

void World::initPositions() {
	mPlayerPaddle->setPosition(mSpawnPosition);
	mBall->setPosition(mPlayerPaddle->getWorldPosition().x, mPlayerPaddle->getWorldPosition().y - mPlayerPaddle->getBoundingRect().height / 2.f - mBall->getBoundingRect().height / 2.f);
	mBall->setVelocity(-150.f, -150.f);
}

CommandQueue& World::getCommandQueue() {
	return mCommandQueue;
}

void World::loadTextures() {
	mTextures.load(Textures::Ball, "./media/breakout_sprites.png");
	mTextures.load(Textures::Bricks, "./media/bricks.png");
	mTextures.load(Textures::Paddle, "./media/paddles.png");
}

void World::recalculateBallPosition() {
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());

	sf::Vector2f ballPosition = mBall->getPosition();
	float ballMidY = mBall->getBoundingRect().height / 2.f;
	float ballMidX = mBall->getBoundingRect().width / 2.f;

	float paddleMidY = mPlayerPaddle->getBoundingRect().height / 2.f;
	float paddleMidX = mPlayerPaddle->getBoundingRect().width / 2.f;

	if (ballPosition.x + ballMidX > viewBounds.width) {
		sf::Vector2f velocity = mBall->getVelocity();
		float newX = -velocity.x;
		mBall->setVelocity(newX, velocity.y);
	}

	if (ballPosition.x - ballMidX < viewBounds.left) {
		sf::Vector2f velocity = mBall->getVelocity();
		float newX = -velocity.x;
		mBall->setVelocity(newX, velocity.y);
	}

	if (ballPosition.y - ballMidY < viewBounds.top) {
		sf::Vector2f velocity = mBall->getVelocity();
		float newY = -velocity.y;
		mBall->setVelocity(velocity.x, newY);
	}

	if (ballPosition.y - ballMidY > viewBounds.height) {
		mBallDown = true;
	}
}

void World::checkPosition() {
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	sf::Vector2f position = mPlayerPaddle->getPosition();
	sf::FloatRect bounds = mPlayerPaddle->getBoundingRect();
	position.x = std::max(position.x, viewBounds.left + bounds.width / 2.f);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - bounds.width / 2.f);
	mPlayerPaddle->setPosition(position);
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2) {
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	if (type1 & category1 && type2 & category2) {
		return true;
	} else if (type1 & category2 && type2 & category1) {
		std::swap(colliders.first, colliders.second);
		return true;
	} else {
		return false;
	}
}

void World::handleCollisions() {
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for (auto pair : collisionPairs) {
		if (matchesCategories(pair, Category::Player, Category::Ball)) {
			auto& ball = static_cast<Ball&>(*pair.second);

			sf::Vector2f currVelocity = ball.getVelocity();
			mBall->setVelocity(currVelocity.x, -currVelocity.y);
		}
		if (matchesCategories(pair, Category::Ball, Category::Brick)) {
			auto& ball = static_cast<Ball&>(*pair.first);
			auto& brick = static_cast<Brick&>(*pair.second);

			auto currVelocity = ball.getVelocity();
			float angle_ball = to_degrees(angle(ball.getWorldPosition(), brick.getWorldPosition()));
			float brick_ratio = to_degrees(std::tan((float) brick.getBoundingRect().height / brick.getBoundingRect().width));
			
			std::cout << "world positions: ball: " << ball.getWorldPosition().x << " brick:" << brick.getWorldPosition().x << std::endl;
			std::cout << "angles: " << angle_ball << " " << brick_ratio << std::endl;

			if (angle_ball > 180 + brick_ratio && angle_ball < 360 - brick_ratio) {
				ball.setVelocity(currVelocity.x, std::abs(currVelocity.y));
			} else if (angle_ball > brick_ratio && angle_ball <= 180 - brick_ratio) {
				ball.setVelocity(currVelocity.x, -std::abs(currVelocity.y));
			} else if (angle_ball > 180 - brick_ratio && angle_ball <= 180 + brick_ratio) {
				ball.setVelocity(std::abs(currVelocity.x), currVelocity.y);
			} else if (angle_ball <= brick_ratio || angle_ball >= 360 - brick_ratio) {
				ball.setVelocity(-std::abs(currVelocity.x), currVelocity.y);
			}
			brick.hit();
		}
	}
}

void World::setLives(int lives) {
	mLives = lives;
}

int World::getLives() {
	return mLives;
}

bool World::getBallDown() const {
	return mBallDown;
}

void World::setBallDown(bool value) {
	mBallDown = value;
}

bool World::checkLevelComplete() const{
	return mSceneGraph.checkLevelComplete();
}

void World::loadNextLevel() {
	std::vector<LevelManager::BrickInfo*> currentLevelInfo = mLevelManager.getCurrentLevelVector();

	for (int i = 0; i < currentLevelInfo.size(); ++i) {
		std::unique_ptr<Brick> brick(new Brick(Brick::getType(currentLevelInfo[i]->type), mTextures.get(Textures::Bricks)));
		brick->setPosition(currentLevelInfo[i]->position);
		mSceneLayers[Foreground]->attachChild(std::move(brick));
	}
}

float angle(const sf::Vector2f& a, const sf::Vector2f& b)
{
	// Y-axis is flipped
	return std::atan2(b.y - a.y, b.x - a.x);
}

float to_degrees(float radians)
{
	// Normalize between 0 and 360
	float degrees = int(radians / 3.14 * 180) % 360;
	return degrees < 0 ? degrees + 360 : degrees;
}

