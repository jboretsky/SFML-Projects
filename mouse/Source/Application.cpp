#include "../Header/Application.hpp"
#include "../Header/World.hpp"

#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(830, 630), "Room!")
, mWorld(mWindow)
{
	mWindow.setFramerateLimit(60);
	mWindow.setKeyRepeatEnabled(false);
}

void Application::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processInput(TimePerFrame);
			update(TimePerFrame);
		}
		render();
	}
}

void Application::processInput(sf::Time dt) {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			mWindow.close();
		}
	}
	mWorld.handleEvent(event,dt);
}

void Application::update(sf::Time dt) {
	mWorld.update(dt);
}

void Application::render() {
	mWindow.clear(sf::Color(155,155,155));
	mWorld.draw();
	mWindow.display();
}