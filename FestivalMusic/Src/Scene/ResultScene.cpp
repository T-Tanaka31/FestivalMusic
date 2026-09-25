#include "ResultScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include <DxLib.h>

ResultScene::ResultScene() {
}

ResultScene::~ResultScene() {
}

void ResultScene::Init() {
}

void ResultScene::Update() {
	if (InputManager::GetInstance()->IsKeyDown(KEY_INPUT_RETURN)) {
		SceneManager::GetInstance()->ChangeScene(SceneType::Title);
	}
}

void ResultScene::Draw() {
	DrawString(100, 100, "ResultScene", GetColor(255, 255, 255));
}