#ifndef TILEMANAGER
#define TILEMANAGER

#include "./Tile.hpp"

#include <vector>

class TileManager: public sf::Drawable {

	public:
		TileManager(sf::FloatRect mWorldRect);
		Tile* getTileAt(sf::Vector2f position) const;
	
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		std::vector<std::vector<Tile*>> mTiles;

		int mColumns;
		int mRows;
};

#endif