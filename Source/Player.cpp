#include "../Header/Player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Player::Player()
: mSprite() {}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) {
	target.draw(mSprite, states);
}