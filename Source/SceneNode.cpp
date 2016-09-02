#include "../Header/SceneNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

SceneNode::SceneNode()
: mChildren()
, mParent(nullptr) {}

void SceneNode::attachChild(Ptr child) {
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {
	auto found = std::find_if(mChildren.begin(), mChildren.end(),
	[&] (Ptr& p) -> bool {return p.get() == &node; });

	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	drawCurrent(target, states);
	for (auto& child : mChildren) {
		child->draw(target, states);
	}
	// drawBoundingRect(target, states);
}

// implemented by others
void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const {
	//Do nothing by default
}

void SceneNode::update(sf::Time dt) {
	updateCurrent(dt);
	updateChildren(dt);
}


// implemented by others
void SceneNode::updateCurrent(sf::Time)
{

}

void SceneNode::updateChildren(sf::Time dt) {
	for(auto& child: mChildren) {
		child->update(dt);
	}
}

sf::Transform SceneNode::getWorldTransform() const {
	sf::Transform transform = sf::Transform::Identity;
	for (const SceneNode* node = this; node != nullptr; node = node->mParent) {
		transform = node->getTransform() * transform;
	}
	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const {
	return getWorldTransform() * sf::Vector2f();
}

void SceneNode::onCommand(const Command& command, sf::Time dt) {
	if (command.category & getCategory()) command.action(*this, dt);

	for (auto &child: mChildren) {
		child->onCommand(command, dt);
	}
}

unsigned int SceneNode::getCategory() const {
	return Category::Scene;
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs) {
	checkNodeCollision(sceneGraph, collisionPairs);

	for (auto& child : sceneGraph.mChildren) {
		checkSceneCollision(*child, collisionPairs);
	}
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs) {
	if (this != &node && collision(*this, node)) {
		// if collision occurs between the nodes, then
		// insert into the collision pairs set
		collisionPairs.insert(std::minmax(this, &node));
	}

	for (auto& child : mChildren) {
		child->checkNodeCollision(node, collisionPairs);
	}
}

sf::FloatRect SceneNode::getBoundingRect() const {
	return sf::FloatRect();
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

bool SceneNode::isDestroyed() const {
	return false;
}

bool SceneNode::isMarkedForRemoval() const {
	return isDestroyed();
}

void SceneNode::removeWrecks() {
	auto wreckfieldBegin = std::remove_if(mChildren.begin(),
		mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());

	std::for_each(mChildren.begin(),
		mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

bool collision(const SceneNode& lhs, const SceneNode& rhs) {
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}
