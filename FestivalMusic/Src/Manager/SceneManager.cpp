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
    : pScene(nullptr)
    , isSceneChange(false)
    , resultType(ResultScene::ResultType::GameOver) {
}

SceneManager::~SceneManager() {
	delete pScene;
}

void SceneManager::ChangeScene(SceneType sceneType) {
	nextScene = sceneType;
	isSceneChange = true;
}

void SceneManager::Update() {
	InputManager::GetInstance()->Update();

	if (pScene) {
		pScene->Update();
	}

	if (isSceneChange) {
		delete pScene;

		switch (nextScene) {
		case SceneType::Title:
			pScene = new TitleScene();
			break;

		case SceneType::Game:
			pScene = new GameScene();
			break;

		case SceneType::Result:
			pScene = new ResultScene();
			((ResultScene*)pScene)->SetResult(resultType);
			break;
		}

		isSceneChange = false;
	}
}


void SceneManager::Draw() {
	if (pScene) {
		pScene->Draw();
	}
}

void SceneManager::ChangeResultScene(ResultScene::ResultType result) {
    resultType = result;
    nextScene = SceneType::Result;
    isSceneChange = true;
}