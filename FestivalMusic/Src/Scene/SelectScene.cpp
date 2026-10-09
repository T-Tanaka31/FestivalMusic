#include "SelectScene.h"

#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Difinition/Constant.h"

#include <DxLib.h>

SelectScene::SelectScene()
	: selectedStage(0)
	, stageCount(5) {
}

SelectScene::~SelectScene() {
}

void SelectScene::Init() {
}

void SelectScene::Update() {
	InputManager* input =
		InputManager::GetInstance();

	// ========================================
	// 左
	// ========================================

	if (input->IsKeyDown(KEY_INPUT_A) ||
		input->IsKeyDown(KEY_INPUT_LEFT)) {
		selectedStage--;

		if (selectedStage < 0) {
			selectedStage = stageCount - 1;
		}
	}

	// ========================================
	// 右
	// ========================================

	if (input->IsKeyDown(KEY_INPUT_D) ||
		input->IsKeyDown(KEY_INPUT_RIGHT)) {
		selectedStage++;

		if (selectedStage >= stageCount) {
			selectedStage = 0;
		}
	}

	// ========================================
	// 決定
	// ========================================

	if (input->IsKeyDown(KEY_INPUT_RETURN)) {
		SceneManager::GetInstance()->ChangeGameScene(
			selectedStage + 1
		);
	}

	// ========================================
	// タイトルへ戻る
	// ========================================

	if (input->IsKeyDown(KEY_INPUT_ESCAPE)) {
		SceneManager::GetInstance()->ChangeScene(
			SceneType::Title
		);
	}
}

void SelectScene::Draw() {
	DrawBox(
		0,
		0,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		GetColor(30, 30, 40),
		TRUE
	);

	DrawString(
		760,
		80,
		"SELECT STAGE",
		GetColor(255, 255, 255)
	);

	for (int i = 0; i < stageCount; i++) {
		int x = 500 + i * 300;
		int y = 280;

		int color;

		if (i == selectedStage) {
			color = GetColor(255, 200, 50);

			DrawBox(
				x - 20,
				y - 20,
				x + 220,
				y + 180,
				color,
				FALSE
			);
		}
		else {
			color = GetColor(180, 180, 180);
		}

		DrawBox(
			x,
			y,
			x + 200,
			y + 160,
			GetColor(80, 80, 100),
			TRUE
		);

		DrawFormatString(
			x + 55,
			y + 65,
			color,
			"STAGE %d",
			i + 1
		);
	}

	DrawString(
		700,
		520,
		"A / D : SELECT",
		GetColor(255, 255, 255)
	);

	DrawString(
		700,
		550,
		"ENTER : START",
		GetColor(255, 255, 255)
	);

	DrawString(
		700,
		580,
		"ESC : TITLE",
		GetColor(255, 255, 255)
	);
}