#include "GameScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include <DxLib.h>
#include "../Difinition/Constant.h"
#include "../Manager/CollisionManager.h"
#include "../Map/MapLoader.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/MapObject/Block.h"
#include "../GameObject/Goal/Goal.h"

GameScene::GameScene()
	: player(new Player(
		VGet(400, 300, 0),
		"Player"))
	, camera(new Camera())
	, goal(nullptr)
	, graphHandle(0) {
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

	for (Enemy* enemy : enemies) {
		delete enemy;
	}

	blocks.clear();
	enemies.clear();
}

void GameScene::Init() {
	graphHandle =
		LoadGraph("Res/fix.png");

	camera->SetTarget(player);

	MapLoader::Load(
		"Res/Map/Stage2.csv",
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

	// 800 x 600 の画面範囲
	float screenLeft = camPos.x - WINDOW_WIDTH * 0.5f;
	float screenRight = camPos.x + WINDOW_WIDTH;
	float screenTop = camPos.y - WINDOW_HEIGHT * 0.5f;
	float screenBottom = camPos.y + WINDOW_HEIGHT;

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

	// Enemy
	for (Enemy* enemy : enemies) {

		VECTOR pos = enemy->GetPosition();

		if (pos.x >= screenLeft &&
			pos.x <= screenRight &&
			pos.y >= screenTop &&
			pos.y <= screenBottom) {

			enemy->Update();
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
	for (Block* block : blocks){
		block->Render();
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

void GameScene::AddEnemy(Enemy* enemy) {
	enemy->SetPlayer(player);

	enemies.push_back(enemy);
}