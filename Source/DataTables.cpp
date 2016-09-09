#include "../Header/DataTables.hpp"
#include "../Header/Pickup.hpp"
#include "../Header/Paddle.hpp"

std::vector<PickupData> initializePickupData() {
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::Enlarge].texture = Textures::Enlarge;
	data[Pickup::Enlarge].action = [] (Paddle& p) { p.enlarge(); };

	return data;
}