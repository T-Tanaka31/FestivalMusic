#include "ResultScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include <DxLib.h>

ResultScene::ResultScene()
	: resultType(ResultType::GameOver) {
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
	if (resultType == ResultType::GameOver) {
		DrawString(
			100,
			100,
			"GAME OVER",
			GetColor(255, 0, 0)
		);
	}
	else {
		DrawString(
			100,
			100,
			"GAME CLEAR",
			GetColor(255, 255, 0)
		);
	}

	DrawString(
		100,
		150,
		"Press ENTER",
		GetColor(255, 255, 255)
	);
}

void ResultScene::SetResult(ResultType type) {
	resultType = type;
}