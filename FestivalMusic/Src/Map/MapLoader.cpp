#include "MapLoader.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

#include "../Scene/GameScene.h"
#include "../GameObject/MapObject/Block.h"
#include "../GameObject/MapObject/MovingBlock.h"
#include "../GameObject/MapObject/Floor.h"
#include "../GameObject/MapObject/Brick.h"
#include "../GameObject/MapObject/Question.h"
#include "../GameObject/MapObject/Rock.h"
#include "../GameObject/MapObject/Spike.h"
#include "../GameObject/MapObject/MovingSpike.h"
#include "../GameObject/MapObject/ReverseSpike.h"
#include "../GameObject/MapObject/MovingRightSpike.h"
#include "../GameObject/MapObject/MovingLeftSpike.h"
#include "../GameObject/Goal/Goal.h"

int MapLoader::bottomBlockY = 0;
int MapLoader::mapLeft = 0;
int MapLoader::mapRight = 0;

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

		// UTF-8 BOMを削除（Stage6.csv対策）
		if (!line.empty() &&
			(unsigned char)line[0] == 0xEF &&
			line.size() >= 3 &&
			(unsigned char)line[1] == 0xBB &&
			(unsigned char)line[2] == 0xBF) {

			line.erase(0, 3);
		}

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
	mapLeft = (std::numeric_limits<int>::max)();
	mapRight = 0;

	//==================================================
	// 通常オブジェクトを生成
	//==================================================

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < (int)map[y].size(); x++) {
			int value = map[y][x];

			// ステージの左右端を取得
			// 1 = Block、10 = Floor
			if (value == 1 || value == 10) {

				int left = x * TILE_SIZE;
				int right = (x + 1) * TILE_SIZE;

				mapLeft = (std::min)(mapLeft, left);
				mapRight = (std::max)(mapRight, right);
			}

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
							TILE_SIZE * 3,
							TILE_SIZE * 3,
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

			case 5:
				scene->AddFlyingEnemy(
					new Enemy(
						pos,
						"Enemy"
					)
				);
				break;

			case 6:
				scene->AddBoss(
					new Boss(pos)
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

			case 19:
				scene->AddReverseSpike(
					new ReverseSpike(
						pos,
						VGet(
							TILE_SIZE,
							TILE_SIZE,
							0
						)
					)
				);
				break;


				// 横移動 64px
			case 20:
				scene->AddMovingBlock(
					new MovingBlock(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveAxis::Horizontal,
						64.0f,
						1.0f
					)
				);
				break;

				// 縦移動 64px
			case 21:
				scene->AddMovingBlock(
					new MovingBlock(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveAxis::Vertical,
						64.0f,
						2.0f
					)
				);
				break;

				// 横移動 128px
			case 22:
				scene->AddMovingBlock(
					new MovingBlock(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveAxis::Horizontal,
						128.0f,
						2.0f
					)
				);
				break;

				// 縦移動 128px
			case 23:
				scene->AddMovingBlock(
					new MovingBlock(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveAxis::Vertical,
						128.0f,
						2.0f
					)
				);
				break;

				// 横移動 192px
			case 24:
				scene->AddMovingBlock(
					new MovingBlock(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveAxis::Horizontal,
						192.0f,
						2.0f
					)
				);
				break;

				// 縦移動 192px
			case 25:
				scene->AddMovingBlock(
					new MovingBlock(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveAxis::Vertical,
						192.0f,
						2.0f
					)
				);
				break;

				// 横移動 256px
			case 26:
				scene->AddMovingBlock(
					new MovingBlock(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveAxis::Horizontal,
						256.0f,
						2.0f
					)
				);
				break;

				// 縦移動 256px
			case 27:
				scene->AddMovingBlock(
					new MovingBlock(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveAxis::Vertical,
						256.0f,
						2.0f
					)
				);
				break;

				// 横移動 64px
			case 30:
				scene->AddMovingRightSpike(
					new MovingRightSpike(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveRightAxis::Horizontal,
						64.0f,
						1.0f
					)
				);
				break;

				// 縦移動 64px
			case 31:
				scene->AddMovingLeftSpike(
					new MovingLeftSpike(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveLeftAxis::Horizontal,
						64.0f,
						1.0f
					)
				);
				break;

			case 50:
				scene->AddMovingSpike(
					new MovingSpike(
						pos,
						VGet(TILE_SIZE, TILE_SIZE, 0),
						MoveDefaultAxis::Horizontal,
						128.0f,
						2.0f
					)
				);
				break;

			}
		}
	}

	// ブロック・床が見つからなかった場合
	if (mapLeft == (std::numeric_limits<int>::max)()) {
		mapLeft = 0;
		mapRight = mapWidth * TILE_SIZE;
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

int MapLoader::GetMapLeft() {
	return mapLeft;
}

int MapLoader::GetMapRight() {
	return mapRight;
}