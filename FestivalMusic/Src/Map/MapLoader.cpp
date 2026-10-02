#include "MapLoader.h"

#include <fstream>
#include <sstream>

#include "../Scene/GameScene.h"
#include "../GameObject/MapObject/Block.h"
#include "../GameObject/MapObject/Floor.h"
#include "../GameObject/MapObject/Brick.h"
#include "../GameObject/MapObject/Question.h"
#include "../GameObject/MapObject/Rock.h"
#include "../GameObject/Goal/Goal.h"

int MapLoader::bottomBlockY = 0;

bool MapLoader::Load(
	const std::string& path,
	GameScene* scene) {

	std::ifstream file(path);

	if (!file.is_open()) {
#if _DEBUG
		printfDx("OPEN FAILED : %s\n", path.c_str());
#endif
		return false;
	}
#if _DEBUG
	printfDx("OPEN SUCCESS\n");
#endif
	std::string line;

	int y = 0;

	constexpr int TILE_SIZE = 64;

	bottomBlockY = 0;

	while (std::getline(file, line)) {

		std::stringstream ss(line);
		std::string cell;

		int x = 0;

		while (std::getline(ss, cell, ',')) {

			try {
				int value = std::stoi(cell);

				VECTOR pos = VGet(
						x * TILE_SIZE + TILE_SIZE / 2.0f,
						y * TILE_SIZE + TILE_SIZE / 2.0f,
						0
				);

				switch (value) {

				case 1:
					scene->AddBlock(
						new Block(
							pos,
							VGet(
								TILE_SIZE,
								TILE_SIZE,
								0
							)
						)
					);
					break;
				case  2:
					scene->SetPlayerPosition(pos);
					break;

				case 3:
					scene->SetGoal(
						new Goal(
							pos,
							VGet(
								TILE_SIZE,
								TILE_SIZE,
								0
							)
						)
					);
					break;

				case 4:
					scene->AddEnemy(
						new Enemy(
							pos,
							"Enemy"
						)
					);
					break;

				case 10:
					scene->AddFloor(
						new Floor(
							pos,
							VGet(
								TILE_SIZE,
								TILE_SIZE,
								0
							)
						)
					);
					// 一番下のブロックの行を記録
					if (y > bottomBlockY) {
						bottomBlockY = y;
					}

					break;

				case 11:
					scene->AddRock(
						new Rock(
							pos,
							VGet(
								TILE_SIZE,
								TILE_SIZE,
								0
							)
						)
					);
					break;

				case 12:
					scene->AddBrick(
						new Brick(
							pos,
							VGet(
								TILE_SIZE,
								TILE_SIZE,
								0
							)
						)
					);
					break;

				case 15:
					scene->AddQues(
						new Question(
							pos,
							VGet(
								TILE_SIZE,
								TILE_SIZE,
								0
							)
						)
					);
					break;
				}


			}
			catch (...) {

				MessageBoxA(
					NULL,
					cell.c_str(),
					"stoi Error",
					MB_OK
				);

				return false;
			}

			x++;
		}

		y++;
	}

#if _DEBUG
	printfDx(
		"Bottom Block Row : %d\n",
		bottomBlockY
	);

	printfDx(
		"Map Bottom Y : %d\n",
		(bottomBlockY + 1) * TILE_SIZE
	);
#endif
	return true;
}

int MapLoader::GetMapBottom() {
	return (bottomBlockY + 1) * 64;
}