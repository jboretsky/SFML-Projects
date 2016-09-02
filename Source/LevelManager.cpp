#include "../Header/LevelManager.hpp"
#include <iostream>
#include <string>

#define LEVEL_SIZE() ((NB_BRICK_COLS + 1) * NB_BRICK_LINES)

LevelManager::BrickInfo::BrickInfo(int type, sf::Vector2f position)
: type(type)
, position(position) {}

LevelManager::LevelManager()
: mCurrentLevel(0)
, mLevelCount() {}

LevelManager::~LevelManager() {
	if (mLevelFile.is_open()) mLevelFile.close();
}

void LevelManager::openFromFile(const std::string& filename) {
	std::cout << "opening " << filename << std::endl;
	mLevelFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
	if (mLevelFile) {
		std::cout << "Successfuly opened the file" << std::endl;
		mLevelFile.seekg(0, std::ifstream::end);
		mLevelCount = mLevelFile.tellg() / LEVEL_SIZE();

		std::cout << mLevelCount << std::endl;

		mLevelFile.seekg(0);
	}
	// throw std::runtime_error("LevelManager::openFromFile - Failed to load " + filename);
};

// void LevelManager::load() {

// };

// void LevelManager::loadNext() {

// };

std::vector<LevelManager::BrickInfo*> LevelManager::getCurrentLevelVector() {
	std::vector<BrickInfo*> bricks;
	std::string line;
	for (int i = 0; i < NB_BRICK_LINES; ++i) {
		std::getline(mLevelFile, line);
		int length = line.size();
		for (int j = 0; j < NB_BRICK_COLS && j < length; ++j) {
			if (line[j] != '.') {
				bricks.push_back(new BrickInfo(line[j] - '0', sf::Vector2f(15 + 66 * j + 33, 20*i + 10)));
			}
		}
	}
	return bricks;
};