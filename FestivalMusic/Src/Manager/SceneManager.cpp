#include "SceneManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
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
		break;
	default:
		break;
	}
}

void SceneManager::Update() {
	if (pScene) {
		pScene->Update();
	}
}

void SceneManager::Draw() {
	if (pScene) {
		pScene->Draw();
	}
}