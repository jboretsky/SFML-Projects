#include "../Header/TileManager.hpp"

TileManager::TileManager(sf::FloatRect mWorldRect)
: mTiles()
, mRows(mWorldRect.height / 20.f)
, mColumns(mWorldRect.width / 20.f) {
	for (int i = 0; i < mRows; ++i) {
		std::vector<Tile*> tiles;
		for (int j = 0; j < mColumns; ++j) {
			tiles.push_back(new Tile(Tile::Ground, sf::Vector2f(mWorldRect.left + j*20, mWorldRect.top + i*20)));
		}
		mTiles.push_back(tiles);
	}
}

Tile* TileManager::getTileAt(sf::Vector2f position) const {
	for(int i = 0; i < mRows; ++i) {
		for(int j = 0; j < mColumns; ++j) {
			sf::FloatRect bounds = mTiles[i][j]->getBounds();
			if (position.x >= bounds.left && position.x < bounds.left + bounds.width) {
				if (position.y >= bounds.top && position.y < bounds.top + bounds.height) {
					return mTiles[i][j];
				}
			}
		}
	}
}

void TileManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (int i = 0; i < mRows; ++i) {
		for (int j = 0; j < mColumns; ++j) {
			target.draw(*mTiles[i][j], states);
		}
	}
}