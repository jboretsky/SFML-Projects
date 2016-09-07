#ifndef CATEGORY
#define CATEGORY

namespace Category {
	enum Type {
		None = 0,
		Scene = 1 << 0,
		Player = 1 << 1,
		Ball = 1 << 2,
		Brick = 1 << 3,
		Unbreakable = 1 << 4,
	};
};

#endif