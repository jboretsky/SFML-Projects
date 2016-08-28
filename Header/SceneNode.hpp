#ifndef SCENENODE
#define SCENENODE

#include "./Category.hpp"
#include "./Command.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>
#include <set>

class SceneNode : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
	public:
		typedef std::unique_ptr<SceneNode> Ptr;

		// stores all collision pairs, is a set because we need to 
		// make sure we don't put the same pair in multiple times
		typedef std::pair<SceneNode*, SceneNode*> Pair;

	public:
		SceneNode();
		void attachChild(Ptr child);
		Ptr detachChild(const SceneNode& node);
		void update(sf::Time dt);
		sf::Vector2f getWorldPosition() const;
		sf::Transform getWorldTransform() const;

		void onCommand(const Command& command, sf::Time dt);
		virtual unsigned int getCategory() const;

		void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
		void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
		virtual sf::FloatRect getBoundingRect() const;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time dt);
		void updateChildren(sf::Time dt);
		void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		std::vector<Ptr> mChildren;
		SceneNode* mParent;
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);

#endif