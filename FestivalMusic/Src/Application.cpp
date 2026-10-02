#include "Manager/InputManager.h" 
#include "Application.h"
#include  <DxLib.h>
#include <ioStream>
#include <random>
#include <memory>
#include "Difinition/Constant.h"

// 静的メンバ変数の初期化
Application* Application::pInstance = nullptr;

void Application::CreateInstance() {
	pInstance = new Application();
}

Application* Application::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void Application::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

Application::Application() 
    : time(0)
	, isGameEnd(true)
	, pSceneManager(SceneManager::GetInstance()) {
}

int Application::Init() {
	std::random_device rd;
	std::mt19937_64 mt(rd());
    SRand(static_cast<int>(mt()));

	pSceneManager->ChangeScene(SceneType::Title);
    return 0;
}

int Application::DxLibInit() {
#if _DEBUG
    SetOutApplicationLogValidFlag(TRUE);
#else
    SetOutApplicationLogValidFlag(FALSE);
#endif
    //  ウィンドウのサイズを変更する
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, FPS);
	//  起動時のウィンドウモードを設定する
    ChangeWindowMode(TRUE);
	//  ウィンドウのタイトルを変更する
	SetWindowText("FestivalMusic");
    //  背景色の設定
	SetBackgroundColor(99,89,214);
	//  DxLibの初期化
    if (DxLib_Init() == -1)
        return 0;

	//  描画する先を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);

	//  図形描画のZバッファを有効化
    {
		//  Zバッファを使用するか
		SetUseZBuffer3D(FALSE);
        //  Zバッファを書き込むかどうか
        SetWriteZBuffer3D(FALSE);
    }

	//	ライティング
	{
		//	ライトの計算をするかどうか		デフォルト : TRUE
		SetUseLighting(TRUE);
		//	標準ライトを使用するかどうか	デフォルト : TRUE
		SetLightEnable(TRUE);
		//	グローバル環境光の設定
		SetGlobalAmbientLight(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));	//	ライトの計算で α値は使わない
	}

    //  XInput対応ゲームパッド設定
	SetUseXInputFlag(TRUE);


    return 0;
}

bool Application::Update() {
	pSceneManager->Update();
	
	if (InputManager::GetInstance()->IsKeyDown(KEY_INPUT_ESCAPE))
		isGameEnd = true;

    return isGameEnd;
}

void Application::Render() {
	pSceneManager->Draw();
}

void Application::ResourceDelete() {
}

void Application::End() {
}

void Application::DxLibEnd() {
	DxLib_End();
}

void Application::Run() {	// 初期化
	int dxLibInitComplete = DxLibInit();
	int initComplete = Init();
	// 初期化に失敗したらゲームを終了する
	isGameEnd = initComplete | dxLibInitComplete;

	//	メインループ
	while (ProcessMessage() == 0) {
		//	終了
		if (isGameEnd) break;
		//	フレーム開始時刻を取得
		time = GetNowCount();
		//	画面をクリアする
		ClearDrawScreen();
		//	更新
		isGameEnd = Update();
		//	描画
		Render();
		//	裏画面と表画面を入れ替える
		ScreenFlip();
		//	Debugrログのクリア
		clsDx();
		// 処理にかかった時間を計算
		int elapsed = GetNowCount() - time;
		int update = int(FRAME_TIME * 1000.0f);
		// 処理が速すぎたら待つ
		if (elapsed < update)
			WaitTimer(update - elapsed);
	}
	// 終了前処理
	ResourceDelete();
	End();
	DxLibEnd();
}
