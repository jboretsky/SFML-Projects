#include "../Header/World.hpp"
#include "../Header/Utility.hpp"

#include <iostream>

World::World(sf::RenderWindow& window) 
: mWindow(window)
, mWorldView(window.getDefaultView())
, mWorldBounds(sf::Vector2f(mWorldView.getSize().x - 30.f, mWorldView.getSize().y - 30.f))
, mWorldRect(sf::FloatRect(15, 15, mWorldBounds.x, mWorldBounds.y))
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 2.f)
, mPlayer(10) {

	centerOrigin(mPlayer);

	std::cout << mWorldRect.left << " " << mWorldRect.top << std::endl;
	for (int j = 0; j < mWorldRect.height / 20; ++j) {
		for (int i = 0; i < mWorldRect.width / 20; ++i) {
			mTiles.push_back(new Tile(Tile::Wall, sf::Vector2f(mWorldRect.left + i*20, mWorldRect.top + j*20)));
		}
	}

	mPlayer.setPosition(mTiles[0]->getPosition());

}

void World::draw() {
	mWindow.setView(mWorldView);
	for (auto tile : mTiles) {
		mWindow.draw(*tile);
	}
	mWindow.draw(mPlayer);
}

void World::update(sf::Time dt) {
	adjustPlayerPosition();
	adjustPlayerVelocity();
}

void World::handleEvent(const sf::Event& event, sf::Time dt) {
	if(event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Right) {
			std::cout << "x: " << sf::Mouse::getPosition(mWindow).x << " y: " << sf::Mouse::getPosition(mWindow).y << std::endl;
			Tile* tile = getTileAt(sf::Vector2f(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y));
			tile->setType(Tile::Ground);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		mPlayer.move(-300.f * dt.asSeconds(), 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		mPlayer.move(0.f,-300.f * dt.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		mPlayer.move(0.f, 300.f * dt.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		mPlayer.move(300.f * dt.asSeconds(), 0.f);
	}
}

void World::adjustPlayerPosition() {
	sf::FloatRect viewBounds = sf::FloatRect(15.f, 15.f, mWorldView.getSize().x - 40.f, mWorldView.getSize().y - 40.f);
	sf::Vector2f playerPosition = mPlayer.getPosition();
	sf::FloatRect playerBounds = mPlayer.getGlobalBounds();

	playerPosition.x = std::max(playerPosition.x - playerBounds.width / 2.f, viewBounds.left);
	playerPosition.x = std::min(playerPosition.x + playerBounds.width / 2.f, viewBounds.left + viewBounds.width);
	playerPosition.y = std::max(playerPosition.y - playerBounds.height / 2.f, viewBounds.top);
	playerPosition.y = std::min(playerPosition.y + playerBounds.height / 2.f, viewBounds.top + viewBounds.height);
	
	mPlayer.setPosition(playerPosition);
}

void World::adjustPlayerVelocity() {

}

Tile* World::getTileAt(sf::Vector2f position) {
	for (auto tile: mTiles) {
		sf::FloatRect bounds = tile->getBounds();
		if (position.x >= bounds.left && position.x < bounds.left + bounds.width) {
			if (position.y >= bounds.top && position.y < bounds.top + bounds.height) {
				return tile;
			}
		}
	}
}