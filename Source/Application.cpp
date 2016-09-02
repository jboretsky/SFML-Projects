#include "../Header/Application.hpp"
#include "../Header/Utility.hpp"
#include "../Header/GameState.hpp"
#include "../Header/PauseState.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(640, 480), "Breakout!")
, mTextures()
, mFonts()
, mPlayer()
, mTextTest()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
{
	mWindow.setFramerateLimit(60);
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::Main, "./media/Sansation.ttf");

	registerStates();
	mStateStack.pushState(States::Game);
}

void Application::run(){
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);
			if (mStateStack.isEmpty())
				mWindow.close();
		}
		render();
	}
}

void Application::processInput() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		mStateStack.handleEvent(event);
		if (event.type == sf::Event::Closed) {
			mWindow.close();
		}
	}
}

void Application::update(sf::Time dt) {
	mStateStack.update(dt);
}

void Application::render() {
	mWindow.clear(sf::Color::Black);
	mStateStack.draw();
	mWindow.display();
}

void Application::registerStates() {
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
}