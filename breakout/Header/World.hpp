#ifndef WORLD
#define WORLD

#include "./ResourceHolder.hpp"
#include "./ResourceIdentifiers.hpp"
#include "./Paddle.hpp"
#include "./Ball.hpp"
#include "./Brick.hpp"
#include "./Player.hpp"
#include "./Pickup.hpp"
#include "./CommandQueue.hpp"
#include "./LevelManager.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>

class World : private sf::NonCopyable {
	public:
		static const int BALL_INIT_VELOCITY = 300;
		static const int MAX_BALL_SPEED = 500;
		static const int BALL_SPEED_STEP = 10;
	public:
		explicit World(sf::RenderWindow& window, FontHolder& fonts);
		void update(sf::Time dt);
		void draw();

		CommandQueue& getCommandQueue();

		void setLives(int lives);
		int getLives();
		void setBallDown(bool value);
		bool getBallDown() const;
		void initPositions();
		bool checkLevelComplete() const;
		void loadNextLevel(sf::Time dt);

	private:
		void loadTextures();
		void buildScene();
		void checkPosition();
		void recalculateBallPosition();
		void handleCollisions();
		void displayPlayerInfo();
		void runCommands(sf::Time dt);
		sf::FloatRect getViewBounds() const;
		sf::FloatRect getBattlefieldBounds() const;
		void destroyEntitiesOutsideView();

	private:
		enum Layer {
			Background,
			Foreground,
			LayerCount,
		};

	private:
		sf::RenderWindow& mWindow;
		sf::View mWorldView;
		TextureHolder mTextures;
		LevelManager mLevelManager;

		SceneNode mSceneGraph;
		std::array<SceneNode*, LayerCount> mSceneLayers;
		CommandQueue mCommandQueue;

		sf::FloatRect mWorldBounds;
		sf::Vector2f mSpawnPosition;
		Paddle* mPlayerPaddle;
		Ball* mBall;
		std::vector<Brick*> mBricks;

		int mLives;
		bool mBallDown;
		int mBricksDestroyed;
		int mBallVelocity;
};

float angle(const sf::Vector2f& a, const sf::Vector2f& b = sf::Vector2f(0.f, 0.f));
float to_degrees(float radians);
float to_radians(float degrees);

#endif
