#include "MapLoader.h"

#include <fstream>
#include <sstream>

#include "../Scene/GameScene.h"
#include "../GameObject/MapObject/Block.h"

bool MapLoader::Load(
	const std::string& path,
	GameScene* scene) {
	std::ifstream file(path);

	if (!file.is_open()) {
		printfDx("OPEN FAILED : %s\n", path.c_str());
		return false;
	}

	printfDx("OPEN SUCCESS\n");

	std::string line;

	int y = 0;

	constexpr int TILE_SIZE = 64;

	while (std::getline(file, line)) {
		std::stringstream ss(line);

		std::string cell;

		int x = 0;

        while (std::getline(ss, cell, ',')) {
            try {
                int value = std::stoi(cell);

                VECTOR pos = VGet(
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    0);

                switch (value) {
                case 1:
                    scene->AddBlock(
                        new Block(
                            pos,
                            VGet(
                                TILE_SIZE,
                                TILE_SIZE,
                                0)));
                    break;

                case 2:
                    scene->SetPlayerPosition(pos);
                    break;
                }
            }
            catch (...) {
                MessageBoxA(
                    NULL,
                    cell.c_str(),
                    "stoi Error",
                    MB_OK);

                return false;
            }

            x++;
        }

		y++;
	}

	return true;
}