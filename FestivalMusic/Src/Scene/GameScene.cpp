#include "GameScene.h"
#include <DxLib.h>
#include "../Difinition/Constant.h"
#include "../Manager/CollisionManager.h"
#include "../Map/MapLoader.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/MapObject/Block.h"

GameScene::GameScene()
	: player(new Player(
		VGet(400, 300, 0),
		"Player"))
	, camera(new Camera())
	, graphHandle(0) {
	Init();
}

GameScene::~GameScene() {
	CollisionManager::GetInstance()->Clear();

	delete player;
	delete camera;

	for (Block* block : blocks) {
		delete block;
	}

	blocks.clear();
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
	player->Update();

	for (Block* block : blocks) {
		block->Update();
	}

	camera->Update();

	CollisionManager::GetInstance()
		->CheckCollision();
}

void GameScene::Draw() {
	for (Block* block : blocks) {
		block->Render();
	}

	player->Render();
}

void GameScene::AddBlock(
	Block* block) {
	blocks.push_back(block);
}