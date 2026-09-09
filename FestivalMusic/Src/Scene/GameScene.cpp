#include "GameScene.h"
#include <DxLib.h>
#include "../Difinition/Constant.h"

GameScene::GameScene()
	: player(new Player(VGet(400, 300, 0), "Player"))
	, graphHandle(0){
	Init();
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	graphHandle = LoadGraph("Res/BackGround.png");
}

void GameScene::Update() {
	player->Update();
}

void GameScene::Draw() {
	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, graphHandle, TRUE);
	DrawString(100, 100, "GameScene", GetColor(255, 255, 255));
	player->Render();
}
