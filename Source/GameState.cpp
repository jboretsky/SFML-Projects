#include "../Header/GameState.hpp"
#include "../Header/Utility.hpp"

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.window, *context.fonts)
, mPlayer(*context.player)
{
	mLivesText.setFont(context.fonts->get(Fonts::Main));
	mLivesText.setString("Lives: " + toString(mWorld.getLives()));
	mLivesText.setCharacterSize(20);
	centerOrigin(mLivesText);
	mLivesText.setPosition(context.window->getSize().x - mLivesText.getLocalBounds().width / 2.f - 10.f, mLivesText.getLocalBounds().height / 2.f + 10.f);
}

void GameState::draw()
{
	mWorld.draw();
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mLivesText);

}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);

	//if mWorld.mBallDown, lose life, and push levelstartstate
	if (mWorld.getBallDown()) {
		mWorld.setBallDown(false);
		mWorld.setLives(mWorld.getLives() - 1);
		mWorld.initPositions();
		requestStackPush(States::Pause);
	}

	bool isComplete = mWorld.checkLevelComplete();
	if (isComplete) {
		mWorld.initPositions();
		mWorld.loadNextLevel();
		requestStackPush(States::LevelComplete);
	}

	mLivesText.setString("Lives: " + toString(mWorld.getLives()));
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);

	//Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		// this add Push to mPendinglist, which is eventually read by the 
		// statestack after updating is done which will push the pause state to the stack
		requestStackPush(States::Pause);

	return true;
}