#pragma once

#include <string>

class GameScene;

class MapLoader {
public:
	static bool Load(
		const std::string& path,
		GameScene* scene
	);

	static int GetMapBottom();

private:
	static int bottomBlockY;
};