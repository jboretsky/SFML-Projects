#ifndef LEVEL
#define LEVEL

#include "./Brick.hpp"
#include <fstream>

struct BrickInfo;

class LevelManager: public sf::NonCopyable {
public:
	struct BrickInfo {
		BrickInfo(int type, sf::Vector2f position);
		int type;
		sf::Vector2f position;
	};
public:
	static const int NB_BRICK_LINES = 15; // number of lines
	static const int NB_BRICK_COLS  = 9; // number of columns
	static const int OFFSET    = 15;  // left offset

	LevelManager();
	~LevelManager();

	void openFromFile(const std::string& filename);
	int getCurrentLevel() const;
	// void load();
	void loadNext();
	std::vector<BrickInfo*> getCurrentLevelVector();

private:
	int 			mCurrentLevel;
	std::fstream 	mLevelFile;
	size_t 			mLevelCount;
};

#endif