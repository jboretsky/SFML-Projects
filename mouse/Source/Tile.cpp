#include "../Header/Tile.hpp"
#include "../Header/Utility.hpp"

Tile::Tile(Type type, sf::Vector2f position)
: mType(type)
, mTile(sf::Vector2f(20,20)) {
	centerOrigin(mTile);
	mTile.setPosition(position.x + mTile.getGlobalBounds().width / 2.f, position.y + mTile.getGlobalBounds().height / 2.f);
	setColor();
}

sf::Vector2f Tile::getPosition() const {
	return mTile.getPosition();
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mTile, states);
	drawBoundingRect(target, states);
}

void Tile::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::FloatRect rect = mTile.getGlobalBounds();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(1.f);

	target.draw(shape, states);
}

sf::FloatRect Tile::getBounds() const {
	return mTile.getGlobalBounds();
}

void Tile::setType(Type type) {
	mType = type;
	setColor();
}

void Tile::setColor() {
	mTile.setFillColor(sf::Color(255*mType, 255*mType, 255*mType));
}