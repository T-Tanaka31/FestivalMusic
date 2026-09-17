#include "SceneManager.h"
#include "InputManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/ResultScene.h"
// 静的メンバ変数の初期化
SceneManager* SceneManager::pInstance = nullptr;

void SceneManager::CreateInstance() {
	pInstance = new SceneManager();
}

SceneManager* SceneManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void SceneManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

SceneManager::SceneManager()
	: pScene(nullptr) {
}

SceneManager::~SceneManager() {
	delete pScene;
}

void SceneManager::ChangeScene(SceneType sceneType) {
	delete pScene;

	switch (sceneType) {
	case SceneType::Title:
		pScene = new TitleScene();
		break;
	case SceneType::Game:
		pScene = new GameScene();
		break;
	case SceneType::Result:
		pScene = new ResultScene();
		break;
	default:
		break;
	}
}

void SceneManager::Update() {
	InputManager::GetInstance()->Update();

	if (InputManager::GetInstance()->IsKeyDown(KEY_INPUT_RETURN)) {
		ChangeScene(SceneType::Game);
	}

	if (InputManager::GetInstance()->IsMouseDown(MOUSE_INPUT_RIGHT)) {
		ChangeScene(SceneType::Result);
	}

	if (InputManager::GetInstance()->IsMouseDown(MOUSE_INPUT_MIDDLE)) {
		ChangeScene(SceneType::Title);
	}

	if (pScene) {
		pScene->Update();
	}
}

void SceneManager::Draw() {
	if (pScene) {
		pScene->Draw();
	}
}