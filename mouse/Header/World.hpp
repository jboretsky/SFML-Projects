#ifndef WORLD
#define WORLD

#include "./Tile.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

class World {
	public:
		World(sf::RenderWindow& window);
		void draw();
		void update(sf::Time dt);

		void handleEvent(const sf::Event& event, sf::Time dt);

	private:
		void adjustPlayerPosition();
		void adjustPlayerVelocity();

	private:
		sf::RenderWindow& mWindow;
		sf::View mWorldView;
		sf::Vector2f mSpawnPosition;

		sf::Vector2f mWorldBounds;
		sf::FloatRect mWorldRect;

		sf::CircleShape mPlayer;
		std::vector<Tile*> mTiles;
};

#endif