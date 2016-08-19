#ifndef GAME
#define GAME

#include "./ResourceHolder.hpp"
#include "./ResourceIdentifiers.hpp"
#include "./World.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Game {
	public:
		Game();
		void run();
		
	private:
		void processInput();
		void update(sf::Time);
		void render();
		void updateStatistics(sf::Time);

	private:
		static const sf::Time TimePerFrame;

		sf::RenderWindow mWindow;
		World mWorld;
		
		TextureHolder mTextures;

		sf::Font mFont;
		sf::Text mStatisticsText;
		sf::Time mStatisticsUpdateTime;
		std::size_t mStatisticsNumFrames;
};

#endif