#ifndef APPLICATION
#define APPLICATION

#include "./World.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Application {
	public:
		Application();
		void run();

	private:
		void processInput(sf::Time dt);
		void update(sf::Time dt);
		void render();

	private:
		static const sf::Time TimePerFrame;
		sf::RenderWindow mWindow;

		World mWorld;
};

#endif