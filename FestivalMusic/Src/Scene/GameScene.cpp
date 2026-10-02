#include "GameScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include <DxLib.h>
#include <string>
#include "../Difinition/Constant.h"
#include "../Manager/CollisionManager.h"
#include "../Map/MapLoader.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/MapObject/Block.h"
#include "../GameObject/MapObject/Floor.h"
#include "../GameObject/MapObject/Question.h"
#include "../GameObject/MapObject/Rock.h"
#include "../GameObject/MapObject/Brick.h"
#include "../GameObject/Goal/Goal.h"

GameScene::GameScene(int _stage)
	: player(new Player(
		VGet(400, 300, 0),
		"Player"))
	, camera(new Camera())
	, goal(nullptr)
	, graphHandle(0)
	, stage(_stage) {
	Init();
}

GameScene::~GameScene() {
	CollisionManager::GetInstance()->Clear();

	delete player;
	delete camera;
	delete goal;

	for (Block* block : blocks) {
		delete block;
	}

	for (Floor* floor : floors) {
		delete floor;
	}

	for (Brick* brick : bricks) {
		delete brick;
	}

	for (Question* ques : queses) {
		delete ques;
	}

	for (Rock* rock : rocks) {
		delete rock;
	}

	for (Enemy* enemy : enemies) {
		delete enemy;
	}

	blocks.clear();
	floors.clear();
	bricks.clear();
	queses.clear();
	rocks.clear();
	enemies.clear();
}

void GameScene::Init() {
	graphHandle =
		LoadGraph("Res/fix.png");

	camera->SetTarget(player);

	std::string mapPath =
		"Res/Map/Stage" +
		std::to_string(stage) +
		".csv";

	MapLoader::Load(
		mapPath,
		this);
}

void GameScene::Update() {

	if (InputManager::GetInstance()->IsKeyDown(KEY_INPUT_RETURN)) {
		SceneManager::GetInstance()->ChangeScene(SceneType::Game);
	}

	// プレイヤーは常に更新
	player->Update();

	// カメラの中心位置
	VECTOR camPos = camera->GetPosition();

	float screenLeft = camPos.x - CAMERA_WIDTH * 0.5f;
	float screenRight = camPos.x + CAMERA_WIDTH;
	float screenTop = camPos.y - CAMERA_HEIGHT * 0.32f;
	float screenBottom = camPos.y + CAMERA_HEIGHT;

	// Block
	for (Block* block : blocks) {

		VECTOR pos = block->GetPosition();

		if (pos.x >= screenLeft &&
			pos.x <= screenRight &&
			pos.y >= screenTop &&
			pos.y <= screenBottom) {

			block->Update();
		}
	}

	for (Floor* floor : floors) {
		VECTOR pos = floor->GetPosition();


		if (pos.x >= screenLeft &&
			pos.x <= screenRight &&
			pos.y >= screenTop &&
			pos.y <= screenBottom) {

			floor->Update();
		}
	}

	for (Brick* brick : bricks) {
		VECTOR pos = brick->GetPosition();


		if (pos.x >= screenLeft &&
			pos.x <= screenRight &&
			pos.y >= screenTop &&
			pos.y <= screenBottom) {

			brick->Update();
		}
	}

	for (Question* ques : queses) {
		VECTOR pos = ques->GetPosition();


		if (pos.x >= screenLeft &&
			pos.x <= screenRight &&
			pos.y >= screenTop &&
			pos.y <= screenBottom) {

			ques->Update();
		}
	}

	for (Rock* rock : rocks) {
		VECTOR pos = rock->GetPosition();


		if (pos.x >= screenLeft &&
			pos.x <= screenRight &&
			pos.y >= screenTop &&
			pos.y <= screenBottom) {

			rock->Update();
		}
	}


	// Enemy
	for (auto it = enemies.begin(); it != enemies.end(); ) {
		Enemy* enemy = *it;

		VECTOR pos = enemy->GetPosition();

		// 死亡中は必ず更新
		if (enemy->IsDying()) {
			enemy->Update();
		}
		// 通常時は画面内だけ更新
		else if (
			pos.x >= screenLeft &&
			pos.x <= screenRight &&
			pos.y >= screenTop &&
			pos.y <= screenBottom) {
			enemy->Update();
		}

		// 死亡アニメーション終了後に削除
		if (enemy->IsDead()) {
			CollisionManager::GetInstance()->RemoveColliders(enemy);

			delete enemy;

			it = enemies.erase(it);
		}
		else {
			++it;
		}
	}

	// Goal
	if (goal != nullptr) {

		VECTOR pos = goal->GetPosition();

		if (pos.x >= screenLeft &&
			pos.x <= screenRight &&
			pos.y >= screenTop &&
			pos.y <= screenBottom) {

			goal->Update();
		}
	}

	camera->Update();

	// Collider同士の衝突判定
	CollisionManager::GetInstance()->CheckCollision();
}
void GameScene::Draw() {
	for (Block* block : blocks) {
		block->Render();
	}
	for (Floor* floor : floors) {
		floor->Render();
	}
	for (Brick* brick : bricks) {
		brick->Render();
	}
	for (Question* ques : queses) {
		ques->Render();
	}
	for (Rock* rock : rocks) {
		rock->Render();
	}
	for (Enemy* enemy : enemies) {
		enemy->Render();
	}

	goal->Render(); player->Render();
}

void GameScene::AddBlock(
	Block* block) {
	blocks.push_back(block);
}

void GameScene::AddFloor(Floor* floor) {
	floors.push_back(floor);
}

void GameScene::AddBrick(Brick* brick) {
	bricks.push_back(brick);
}

void GameScene::AddQues(Question* ques) {
	queses.push_back(ques);
}

void GameScene::AddRock(Rock* rock) {
	rocks.push_back(rock);
}

void GameScene::AddEnemy(Enemy* enemy) {
	enemy->SetPlayer(player);

	enemies.push_back(enemy);
}