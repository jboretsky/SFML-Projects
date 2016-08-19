#include "../Header/Application.hpp"
#include "../Header/Utility.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

Game::Game()
: mWindow(sf::VideoMode(640, 480), "Breakout!")
, mTextures()
, mWorld(mWindow)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setPosition(sf::Vector2i(10,50));
}

void Game::run(){
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);
		}
		// updateStatistics(elapsedTime);
		render();
	}
}

void Game::processInput() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		mWorld.handleEvent(event);
		if (event.type == sf::Event::Closed) {
			mWindow.close();
		}
	}
}

void Game::update(sf::Time dt) {
	mWorld.update(dt);
}

void Game::render() {
	mWindow.clear(sf::Color::Black);
	mWorld.draw();
	mWindow.display();
}

// void updateStatistics(sf::Time);