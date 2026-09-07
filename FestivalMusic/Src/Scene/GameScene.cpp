#include "GameScene.h"
#include <DxLib.h>

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
}

void GameScene::Update() {
}

void GameScene::Draw() {
	DrawString(100, 100, "GameScene", GetColor(255, 255, 255));
}
