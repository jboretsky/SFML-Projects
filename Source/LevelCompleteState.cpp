#include "../Header/LevelCompleteState.hpp"
#include "../Header/Utility.hpp"
#include "../Header/ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

LevelCompleteState::LevelCompleteState(StateStack& stack, Context context)
: State(stack, context)
, mBackgroundSprite()
, mPausedText() {
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mPausedText.setFont(font);
	mPausedText.setString("Level Complete\nPress any key to begin the next level.");	
	mPausedText.setCharacterSize(25);
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void LevelCompleteState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0,0,0,150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPausedText);
}

bool LevelCompleteState::update(sf::Time) {
	return false;
}

bool LevelCompleteState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		requestStackPop();
	}
	return false;
}