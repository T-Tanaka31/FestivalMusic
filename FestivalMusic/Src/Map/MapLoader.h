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

	static int GetMapLeft();
	static int GetMapRight();

private:
	static int bottomBlockY;
	static int mapLeft;
	static int mapRight;
};