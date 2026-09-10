#pragma once
#include "Manager/SceneManager.h"
#include "Component/Collider.h"

constexpr double FRAME_TIME = 1.0 / 60.0;

class Application {
#pragma region singleton
private:
	static Application* pInstance;

private:
	/*
	 *	@brief	コンストラクタ
	 *	@tips	外部で生成されないようにアクセス指定子をprivateにする
	 */
	Application();

	/*
	 *	@brief	デストラクタ
	 */
	~Application() = default;

public:
	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator = (Application&&) = delete;

private:	//	静的メンバ関数
	/*
	 *	@function	CreateInstance
	 *	@brief		自身のインスタンスを生成する
	 */
	static void CreateInstance();

public:		//	静的メンバ関数
	/*
	 *	@function	GetInstance
	 *	@brief		自身のインスタンスを取得する唯一の手段
	 *	@return		Application*	自身のインスタンスのアドレス
	 */
	static Application* GetInstance();

	/*
	 *	@function	DestroyInstance
	 *	@brief		自身のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion

private:
	//	FPS調整用
	int time;
	//	ゲーム終了フラグ
	bool isGameEnd;

	SceneManager* pSceneManager;

private:
	//	初期化
	int Init();

	//	DxLibの初期化
	int DxLibInit();

	//	更新
	bool Update();

	//	描画
	void Render();

	//	Resourceの削除
	void ResourceDelete();

	//	終了前処理
	void End();

	//	DxLibの終了処理
	void DxLibEnd();

public:
	//	ゲームループ
	void Run();

	//	ゲーム終了
	void GameEnd() { isGameEnd = true; }
};

