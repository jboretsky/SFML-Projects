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
, mRows(mWorldRect.height / 20)
, mColumns(mWorldRect.width / 20) {
	mPlayer = new Player();
	std::cout << mWorldRect.left << " " << mWorldRect.top << std::endl;
	for (int j = 0; j < mRows; ++j) {
		std::vector<Tile*> tiles;
		for (int i = 0; i < mColumns; ++i) {
			tiles.push_back(new Tile(Tile::Ground, sf::Vector2f(mWorldRect.left + i*20, mWorldRect.top + j*20)));
		}
		mTiles.push_back(tiles);
	}

	mPlayer->setPosition(mTiles[0][0]->getPosition());
}

void World::draw() {
	mWindow.setView(mWorldView);
	for (int i = 0; i < mRows; ++i) {
		for (int j = 0; j < mColumns; ++j) {
			mWindow.draw(*mTiles[i][j]);
		}
	}
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
				Tile* tile = getTileAt(sf::Vector2f(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y));
				mPlayer->moveTo(tile->getPosition());
			}
		}
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (mWorldRect.contains(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y)) {
				Tile* tile = getTileAt(sf::Vector2f(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y));
				tile->setType(Tile::Wall);
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		mPlayer->setVelocity(-300.f * dt.asSeconds(), 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		mPlayer->setVelocity(0.f,-300.f * dt.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		mPlayer->setVelocity(0.f, 300.f * dt.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		mPlayer->setVelocity(300.f * dt.asSeconds(), 0.f);
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

Tile* World::getTileAt(sf::Vector2f position) {
	for(int i = 0; i < mRows; ++i) {
		for(int j = 0; j < mColumns; ++j) {
			sf::FloatRect bounds = mTiles[i][j]->getBounds();
			if (position.x >= bounds.left && position.x < bounds.left + bounds.width) {
				if (position.y >= bounds.top && position.y < bounds.top + bounds.height) {
					return mTiles[i][j];
				}
			}
		}
	}
}