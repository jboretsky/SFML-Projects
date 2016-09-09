#ifndef LEVELCOMPLETESTATE
#define LEVELCOMPLETESTATE

#include "./State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class LevelCompleteState : public State {
	public:
		LevelCompleteState(StateStack& stack, Context context);
		
		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Sprite mBackgroundSprite;
		sf::Text mPausedText;
};

#endif