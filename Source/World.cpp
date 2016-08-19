#include "../Header/World.hpp"
#include "../Header/ResourceIdentifiers.hpp"
#include "../Header/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mPaddle()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 1.05f) {
	loadTextures();
	buildScene();
	mPaddle.setTexture(mTextures.get(Textures::Paddle));
	centerOrigin(mPaddle);
	mPaddle.setPosition(mSpawnPosition);
}

void World::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mPaddle);
}

void World::update(sf::Time dt) {
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		sf::Vector2f movement(-5.f, 0.f);
		mPaddle.move(movement);
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		sf::Vector2f movement(5.f, 0.f);
		mPaddle.move(movement);
	}
	checkPosition();
}

void World::handleEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		sf::Vector2f movement(1.f,0.f);
		mPaddle.move(movement);
	}
}

void World::buildScene() {

}

void World::loadTextures() {
	mTextures.load(Textures::Paddle, "./media/paddle.png");
}

void World::checkPosition() {
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	sf::Vector2f position = mPaddle.getPosition();
	sf::FloatRect bounds = mPaddle.getGlobalBounds();
	position.x = std::max(position.x, viewBounds.left + bounds.width / 2.f);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - bounds.width / 2.f);
	mPaddle.setPosition(position);
}