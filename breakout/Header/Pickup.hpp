#ifndef PICKUP
#define PICKUP

#include "./Entity.hpp"
#include "./ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Paddle;

class Pickup : public Entity {
	public:
		enum Type {
			Enlarge,
			Gun,
			TypeCount,
		};

	public:
		Pickup(Type type, const TextureHolder& textures);

		virtual unsigned int getCategory() const;
		virtual sf::FloatRect getBoundingRect() const;

		void apply(Paddle& player) const;
		virtual void destroy();

	protected:
		virtual bool isMarkedForRemoval() const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		Type mType;
		sf::Sprite mSprite;
		bool mIsMarkedForRemoval;
};

#endif