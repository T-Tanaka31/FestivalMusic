#include "TitleScene.h"
#include <DxLib.h>

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Init() {
}

void TitleScene::Update() {
}

void TitleScene::Draw() {
	DrawString(100, 100, "TitleScene", GetColor(255, 255, 255));
}
