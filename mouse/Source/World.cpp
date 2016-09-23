#include "../Header/World.hpp"
#include "../Header/Utility.hpp"

#include <iostream>

World::World(sf::RenderWindow& window) 
: mWindow(window)
, mWorldView(window.getDefaultView())
, mWorldBounds(sf::Vector2f(mWorldView.getSize().x - 30.f, mWorldView.getSize().y - 30.f))
, mWorldRect(sf::FloatRect(15, 15, mWorldBounds.x, mWorldBounds.y))
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 2.f)
, mPlayer(NULL)
, mTileManager(mWorldRect) {
	mPlayer = new Player();

	mPlayer->setPosition(0.f,0.f);
}

void World::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mTileManager);
	mWindow.draw(*mPlayer);
}

void World::update(sf::Time dt) {
	mPlayer->update(dt);
	mPlayer->setVelocity(0.f, 0.f);
	adjustPlayerPosition();
	adjustPlayerVelocity();
}

void World::handleEvent(const sf::Event& event, sf::Time dt) {
	if(event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Right) {
			if (mWorldRect.contains(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y)) {
				Tile* tile = mTileManager.getTileAt(sf::Vector2f(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y));
				mPlayer->moveTo(tile->getPosition());
			}
		}
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (mWorldRect.contains(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y)) {
				Tile* tile = mTileManager.getTileAt(sf::Vector2f(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y));
				tile->setType(Tile::Wall);
			}
		}
	}
}

void World::adjustPlayerPosition() {
	sf::FloatRect viewBounds = sf::FloatRect(15.f, 15.f, mWorldView.getSize().x - 40.f, mWorldView.getSize().y - 40.f);
	sf::Vector2f playerPosition = mPlayer->getPosition();
	sf::FloatRect playerBounds = mPlayer->getBounds();

	playerPosition.x = std::max(playerPosition.x - playerBounds.width / 2.f, viewBounds.left);
	playerPosition.x = std::min(playerPosition.x + playerBounds.width / 2.f, viewBounds.left + viewBounds.width);
	playerPosition.y = std::max(playerPosition.y - playerBounds.height / 2.f, viewBounds.top);
	playerPosition.y = std::min(playerPosition.y + playerBounds.height / 2.f, viewBounds.top + viewBounds.height);
	
	mPlayer->setPosition(playerPosition);
}

void World::adjustPlayerVelocity() {

}