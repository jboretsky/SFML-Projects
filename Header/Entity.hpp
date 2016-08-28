#ifndef ENTITY
#define ENTITY

#include "./SceneNode.hpp"

// entitys are objects (that can move)
// our ball will be an entity
// and our paddle will also be an entity
// we simply need to define a sprite in our actual implementations
// and that is all
class Entity : public SceneNode {
	public:
		Entity();

		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float vx, float vy);
		sf::Vector2f getVelocity() const;
	private:
		virtual void updateCurrent(sf::Time dt);
	private:
		sf::Vector2f mVelocity;
};

#endif