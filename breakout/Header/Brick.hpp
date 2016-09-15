#ifndef BRICK
#define BRICK

#include "./Entity.hpp"
#include "./Command.hpp"
#include "./CommandQueue.hpp"
#include "./ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Brick : public Entity {
	public:
		enum brickLayer {
			None,
			One,
			Two,
			Three,
			Unbreakable,
		};

	public:
		Brick(brickLayer layer, TextureHolder& textures);
		virtual unsigned int getCategory() const;

		virtual sf::FloatRect getBoundingRect() const;
		sf::IntRect getTypeCoords(brickLayer layer);

		void hit();
		static Brick::brickLayer getType(int i);
		Brick::brickLayer getType() const;
		virtual bool isMarkedForRemoval() const;

	private:
		virtual bool isDestroyed() const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

		virtual void destroy();
		void checkPickupDrop(CommandQueue& commands);
		void createPickup(SceneNode& node, const TextureHolder& textures) const;

	private:
		sf::Sprite mSprite;
		brickLayer mBrickLayer;
		Command mDropPickupCommand;
		bool mIsMarkedForRemoval;
};

#endif