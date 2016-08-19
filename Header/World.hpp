#ifndef WORLD
#define WORLD

#include "./ResourceHolder.hpp"
#include "./ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

class World : private sf::NonCopyable {
	public:
		explicit World(sf::RenderWindow& window);
		void update(sf::Time dt);
		void draw();
		void handleEvent(sf::Event& event);

	private:
		void loadTextures();
		void buildScene();
		void checkPosition();
	private:
		sf::RenderWindow& mWindow;
		sf::View mWorldView;
		TextureHolder mTextures;

		sf::FloatRect mWorldBounds;
		sf::Vector2f mSpawnPosition;
		sf::Sprite mPaddle;
};

#endif
