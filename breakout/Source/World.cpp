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
, mBricksDestroyed(0)
, mBallVelocity(BALL_INIT_VELOCITY)
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

	destroyEntitiesOutsideView();

	runCommands(dt);

	handleCollisions();
	mSceneGraph.update(dt, mCommandQueue);

	mSceneGraph.removeWrecks();
	checkPosition();
	recalculateBallPosition();
}

void World::runCommands(sf::Time dt) {
	while(!mCommandQueue.isEmpty()) {
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
}

void World::buildScene() {
	for(std::size_t i = 0; i < LayerCount; ++i) {
		Category::Type category = (i == Foreground) ? Category::Scene : Category::None; 
		SceneNode::Ptr layer(new SceneNode(category));
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
		std::unique_ptr<Brick> brick(new Brick(Brick::getType(currentLevelInfo[i]->type), mTextures));
		brick->setPosition(currentLevelInfo[i]->position);
		mSceneLayers[Foreground]->attachChild(std::move(brick));
	}
}

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
	mTextures.load(Textures::Enlarge, "./media/enlarge.png");
}

void World::recalculateBallPosition() {
	sf::FloatRect viewBounds = getViewBounds();
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
	sf::FloatRect viewBounds = getViewBounds();
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
			auto& paddle = static_cast<Paddle&>(*pair.first);
			auto& ball = static_cast<Ball&>(*pair.second);

			float x = ball.getWorldPosition().x - paddle.getWorldPosition().x; // x will be 0 if it hits right in the center, negative to the left, positive to the right
			// std::cout << "X position hit: "<< x << std::endl;
			float range = 180 - 30 * 2; // range is 120;
			float degrees = (range * x / paddle.getBoundingRect().width) + 90;
			// std::cout << "Degrees: " << degrees << std::endl;
			float ball_new_angle = 3.14 - to_radians(degrees);
			// std::cout << "New radians: " << ball_new_angle << std::endl;

			auto newVelocity = mBallVelocity + (mBricksDestroyed * BALL_SPEED_STEP);
			if (newVelocity < MAX_BALL_SPEED)
				mBallVelocity = newVelocity;
			else {
				mBallVelocity = MAX_BALL_SPEED;
			}

			mBricksDestroyed = 0;

			mBall->setVelocity(mBallVelocity * std::cos(ball_new_angle), -mBallVelocity * std::sin(ball_new_angle));
		}
		if (matchesCategories(pair, Category::Ball, Category::Brick) || matchesCategories(pair, Category::Ball, Category::Unbreakable)) {
			auto& ball = static_cast<Ball&>(*pair.first);
			auto& brick = static_cast<Brick&>(*pair.second);

			auto currVelocity = ball.getVelocity();
			float angle_ball = to_degrees(angle(ball.getWorldPosition(), brick.getWorldPosition()));
			float brick_ratio = to_degrees(std::tan((float) brick.getBoundingRect().height / brick.getBoundingRect().width));

			if (brick.getType() == 1) {
				mBricksDestroyed += 1;
			}
			
			// std::cout << "angle of brick to ball: " << angle_ball << ", brick ratio:" << brick_ratio << std::endl;

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
		if (matchesCategories(pair, Category::Player, Category::Pickup)) {
			auto& paddle = static_cast<Paddle&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			pickup.apply(paddle);
			pickup.destroy();
		}
	}
}


void World::destroyEntitiesOutsideView() {
	Command command;
	command.category = Category::Pickup;
	command.action = derivedAction<Entity>([this] (Entity& e, sf::Time) {
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.destroy();
	});

	mCommandQueue.push(command);
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

void World::loadNextLevel(sf::Time dt) {

	Command command;
	command.category = Category::Unbreakable | Category::Pickup;
	command.action = derivedAction<Entity>([this] (Entity& e, sf::Time) {
		e.destroy();
	});

	mCommandQueue.push(command);

	runCommands(dt);
	mSceneGraph.removeWrecks();
	mBallVelocity = BALL_INIT_VELOCITY;

	std::vector<LevelManager::BrickInfo*> currentLevelInfo = mLevelManager.getCurrentLevelVector();

	for (int i = 0; i < currentLevelInfo.size(); ++i) {
		std::unique_ptr<Brick> brick(new Brick(Brick::getType(currentLevelInfo[i]->type), mTextures));
		brick->setPosition(currentLevelInfo[i]->position);
		mSceneLayers[Foreground]->attachChild(std::move(brick));
	}
}

float to_radians(float degrees) {
	return degrees * 3.14 / 180;
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

sf::FloatRect World::getBattlefieldBounds() const {
	sf::FloatRect bounds = getViewBounds();
	return bounds;
}

sf::FloatRect World::getViewBounds() const {
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}
