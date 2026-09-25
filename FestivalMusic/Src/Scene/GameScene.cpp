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
	player->Update();

	for (Block* block : blocks) {
		block->Update();
	}

	for (Enemy* enemy : enemies) {
		enemy->Update();
	}

	goal->Update();

	camera->Update();

	CollisionManager::GetInstance()
		->CheckCollision();
}

void GameScene::Draw() {
	for (Block* block : blocks) {
		block->Render();
	}
	for (Enemy* enemy : enemies) {
		enemy->Render();
	}
	goal->Render();
	player->Render();
}

void GameScene::AddBlock(
	Block* block) {
	blocks.push_back(block);
}

void GameScene::AddEnemy(Enemy* enemy) {
	enemies.push_back(enemy);
}