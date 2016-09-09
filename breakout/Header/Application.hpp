#ifndef APPLICATION
#define APPLICATION

#include "./ResourceHolder.hpp"
#include "./ResourceIdentifiers.hpp"
#include "./Player.hpp"
#include "./StateStack.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Application {
	public:
		Application();
		void run();
		
	private:
		void processInput();
		void update(sf::Time);
		void render();
		
		void registerStates();

	private:
		static const sf::Time TimePerFrame;

		sf::RenderWindow mWindow;
		TextureHolder mTextures;
		FontHolder mFonts;
		Player mPlayer;

		sf::Text mTextTest;

		StateStack mStateStack;
};

#endif