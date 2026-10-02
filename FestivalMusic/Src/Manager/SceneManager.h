#pragma once

#include "../Scene/Scene.h"
#include "../Scene/ResultScene.h"
#include "../Scene/SelectScene.h"

enum class SceneType {
	Title,
	Select,
	Game,
	Result
};

class SceneManager {
private:
	static SceneManager* pInstance;

	Scene* pScene;
	SceneType nextScene;
	bool isSceneChange;

	ResultScene::ResultType resultType;

	int selectedStage;

	SceneManager();
	~SceneManager();
public:
	static void CreateInstance();
	static SceneManager* GetInstance();
	static void DestroyInstance();

	void ChangeScene(SceneType sceneType);

	// ResultScene用
	void ChangeResultScene(ResultScene::ResultType result);

	void ChangeGameScene(int stage);

	void Update();
	void Draw();
};