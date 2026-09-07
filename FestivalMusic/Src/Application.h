#pragma once
//	アプリケーションクラス

constexpr double FRAME_TIME = 1.0 / 60.0;

class Application {
private:
	//	FPS調整用
	int time;
	//	ゲーム終了フラグ
	bool isGameEnd;
public:
	Application();
	~Application() = default;
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

