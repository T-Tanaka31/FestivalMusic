
#include "ResultScene.h"
#include "../Difinition/Constant.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include <DxLib.h>

// ========================================
// コンストラクタ
// ========================================

ResultScene::ResultScene()
    : resultType(ResultType::GameOver)
    , graphHandle(-1) {
    Init();
}

ResultScene::~ResultScene() {
}

void ResultScene::Init() {
    graphHandle = LoadGraph("Res/StageClear.png");
}

// ========================================
// 更新処理
// ========================================

void ResultScene::Update() {

    if (
        InputManager::GetInstance()->IsKeyDown(KEY_INPUT_RETURN) ||
        InputManager::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A)
    ) {
        SceneManager::GetInstance()->ChangeScene(SceneType::Title);
    }
}

// ========================================
// 描画処理
// ========================================

void ResultScene::Draw() {

    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;

    if (resultType == ResultType::GameOver) {

        // 背景
        DrawBox(
            0, 0,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            GetColor(13, 8, 25),
            TRUE
        );

        // 暗い赤の帯
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

        for (int i = 0; i < 8; i++) {
            int y = i * 90;

            DrawBox(
                0, y,
                WINDOW_WIDTH, y + 25,
                GetColor(100, 0, 35),
                TRUE
            );
        }

        // ノイズのような赤い線
        for (int i = 0; i < 18; i++) {
            int y = (i * 67 + 31) % WINDOW_HEIGHT;
            int x = (i * 113) % WINDOW_WIDTH;
            int width = 30 + (i * 47) % 180;

            DrawBox(
                x, y,
                x + width, y + 3,
                GetColor(220, 0, 70),
                TRUE
            );
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // 中央のパネル
        DrawBox(
            centerX - 260,
            centerY - 125,
            centerX + 260,
            centerY + 125,
            GetColor(25, 13, 35),
            TRUE
        );

        // パネルの枠
        DrawBox(
            centerX - 260,
            centerY - 125,
            centerX + 260,
            centerY + 125,
            GetColor(160, 25, 65),
            FALSE
        );

        // 上下の装飾線
        DrawLine(
            centerX - 220, centerY - 85,
            centerX + 220, centerY - 85,
            GetColor(180, 35, 75), 2
        );

        DrawLine(
            centerX - 220, centerY + 75,
            centerX + 220, centerY + 75,
            GetColor(180, 35, 75), 2
        );

        // GAME OVER
        SetFontSize(48);

        DrawString(
            centerX - 150 + 3,
            centerY - 55 + 3,
            "GAME OVER",
            GetColor(60, 0, 15)
        );

        DrawString(
            centerX - 150,
            centerY - 55,
            "GAME OVER",
            GetColor(255, 45, 85)
        );

        // サブタイトル
        SetFontSize(20);

        DrawString(
            centerX - 105,
            centerY + 15,
            "THE NOISE HAS WON...",
            GetColor(190, 150, 175)
        );

        // 操作案内
        SetFontSize(20);

        DrawString(
            centerX - 90,
            centerY + 165,
            "PRESS ENTER",
            GetColor(255, 255, 255)
        );

        DrawString(
            centerX - 65,
            centerY + 195,
            "TO TITLE",
            GetColor(170, 150, 180)
        );
    }

    // ========================================
    // ゲームクリア
    // ========================================

    else if (resultType == ResultType::GameClear) {

        DrawExtendGraph(
            0, 0,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            graphHandle,
            TRUE
        );
    }

    // フォントサイズを戻す
    SetFontSize(16);
}

// ========================================
// 結果設定
// ========================================

void ResultScene::SetResult(ResultType type) {
    resultType = type;
}
