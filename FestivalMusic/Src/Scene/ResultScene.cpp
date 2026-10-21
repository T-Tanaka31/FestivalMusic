#include "ResultScene.h"
#include "../Difinition/Constant.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include <DxLib.h>

ResultScene::ResultScene()
	: resultType(ResultType::GameOver)
	, graphHandle(-1)
{
	Init();
}

ResultScene::~ResultScene() {
}

void ResultScene::Init() {
	graphHandle =
		LoadGraph("Res/StageClear.png");
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
	else if(resultType == ResultType::GameClear) {
	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, graphHandle, TRUE);
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