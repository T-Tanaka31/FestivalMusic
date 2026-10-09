#include "MapLoader.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "../Scene/GameScene.h"
#include "../GameObject/MapObject/Block.h"
#include "../GameObject/MapObject/Floor.h"
#include "../GameObject/MapObject/Brick.h"
#include "../GameObject/MapObject/Question.h"
#include "../GameObject/MapObject/Rock.h"
#include "../GameObject/MapObject/Spike.h"
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

	constexpr int TILE_SIZE = 64;

	//==================================================
	// CSVを一旦全部読み込む
	//==================================================

	std::vector<std::vector<int>> map;

	std::string line;

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;

		std::vector<int> row;

		while (std::getline(ss, cell, ',')) {
			try {
				row.push_back(std::stoi(cell));
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
		}

		map.push_back(row);
	}

	//==================================================
	// マップサイズ
	//==================================================

	const int mapHeight = (int)map.size();

	if (mapHeight == 0) {
		return false;
	}

	const int mapWidth = (int)map[0].size();

	bottomBlockY = 0;

	//==================================================
	// 通常オブジェクトを生成
	//==================================================

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < (int)map[y].size(); x++) {
			int value = map[y][x];

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


			case 2:

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

			case 18:
				scene->AddSpike(
					new Spike(
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
	}


	//==================================================
	// Floorを横方向にまとめる
	//==================================================

	for (int y = 0; y < mapHeight; y++) {
		int x = 0;

		while (x < (int)map[y].size()) {
			// Floorではない
			if (map[y][x] != 10) {
				x++;
				continue;
			}

			// 連続する10の開始位置
			int startX = x;

			// 連続する10を探す
			while (
				x < (int)map[y].size() &&
				map[y][x] == 10) {
				x++;
			}

			// 個数
			int count = x - startX;

			//==========================================
			// 横長Floorを1個作る
			//==========================================

			float width = TILE_SIZE * count;

			float centerX =
				startX * TILE_SIZE +
				width * 0.5f;

			float centerY =
				y * TILE_SIZE +
				TILE_SIZE * 0.5f;

			VECTOR pos = VGet(
				centerX,
				centerY,
				0
			);

			VECTOR size = VGet(
				width,
				TILE_SIZE,
				0
			);

			scene->AddFloor(
				new Floor(
					pos,
					size
				)
			);

			// 一番下のFloor
			if (y > bottomBlockY) {
				bottomBlockY = y;
			}
		}
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