#ifndef DATATABLES
#define DATATABLES

#include "./ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>

class Paddle;

struct PickupData {
	std::function<void(Paddle&)> action;
	Textures::ID texture;
};

std::vector<PickupData> initializePickupData();

#endif