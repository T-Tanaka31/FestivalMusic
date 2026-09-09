#include "TitleScene.h"
#include <DxLib.h>
#include "../Difinition/Constant.h"

TitleScene::TitleScene()
	: graphHandle(0){
	Init();
}

TitleScene::~TitleScene() {
}

void TitleScene::Init() {
	graphHandle = LoadGraph("Res/Title.png");
}

void TitleScene::Update() {
}

void TitleScene::Draw() {
	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, graphHandle, TRUE);
	DrawString(100, 100, "TitleScene", GetColor(255, 255, 255));
}
