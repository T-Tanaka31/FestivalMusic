#include "TitleScene.h"
#include "../Difinition/Constant.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include <DxLib.h>

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
	if (InputManager::GetInstance()->IsKeyDown(KEY_INPUT_RETURN)) {
		SceneManager::GetInstance()->ChangeScene(SceneType::Game);
	}
}

void TitleScene::Draw() {
	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, graphHandle, TRUE);
	DrawString(100, 100, "TitleScene", GetColor(255, 255, 255));
}
