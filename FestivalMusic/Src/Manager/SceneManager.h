#pragma once

#include "../Scene/Scene.h"
#include "../Scene/ResultScene.h"

enum class SceneType {
	Title,
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

	SceneManager();
	~SceneManager();
public:
	static void CreateInstance();
	static SceneManager* GetInstance();
	static void DestroyInstance();

	void ChangeScene(SceneType sceneType);

	// ResultScene用
	void ChangeResultScene(ResultScene::ResultType result);

	void Update();
	void Draw();
};