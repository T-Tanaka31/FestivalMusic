
#include "NoiseEffect.h"
#include "../../Difinition/Constant.h"
#include "../../Manager/AudioManager.h"
#include <DxLib.h>
#include <cstdlib>
#include <algorithm>

// ========================================
// コンストラクタ
// ========================================

NoiseEffect::NoiseEffect() {
    elapsedFrames = 0;
    nextGlitchFrame = 100;
    glitchDuration = 0;

    screenWidth = WINDOW_WIDTH;
    screenHeight = WINDOW_HEIGHT;

    deathActive = false;
    deathFrame = 0;
}

// ========================================
// 更新処理
// ========================================

void NoiseEffect::Update(int stage, int playerHP) {

    // HPを0～5に制限
    playerHP = (std::max)(0, (std::min)(playerHP, 5));

    // ========================================
    // ステージによるノイズ発生頻度
    // ========================================

    int minInterval = 90;
    int maxInterval = 160;

    if (stage >= 4) {
        minInterval = 60;
        maxInterval = 110;
    }

    if (stage >= 6) {
        minInterval = 40;
        maxInterval = 80;
    }

    // ========================================
    // HPによるノイズ発生頻度
    // ========================================

    if (playerHP <= 1) {
        minInterval -= 30;
        maxInterval -= 40;
    }
    else if (playerHP <= 2) {
        minInterval -= 25;
        maxInterval -= 30;
    }
    else if (playerHP <= 3) {
        minInterval -= 15;
        maxInterval -= 20;
    }
    else if (playerHP <= 4) {
        minInterval -= 0;
        maxInterval -= 0;
    }

    minInterval = (std::max)(minInterval, 8);
    maxInterval = (std::max)(maxInterval, minInterval + 1);

    // ========================================
    // 発生中のノイズを管理
    // ========================================

    if (glitchDuration > 0) {
        glitchDuration--;

        if (glitchDuration == 0) {
            blocks.clear();
        }

        return;
    }

    elapsedFrames++;

    if (elapsedFrames < nextGlitchFrame) {
        return;
    }

    elapsedFrames = 0;

    nextGlitchFrame = minInterval +
        (std::rand() % (maxInterval - minInterval + 1));

    // ========================================
    // ノイズの表示時間
    // ========================================

    int duration = 2 + (std::rand() % 4);

    if (playerHP <= 4) duration += 2;
    if (playerHP <= 3) duration += 2;
    if (playerHP <= 2) duration += 3;
    if (playerHP <= 1) duration += 4;

    glitchDuration = duration;

    // HPに応じてノイズ音量を変更
    float noiseVolume = 1.0f;

    if (playerHP == 5) {
        noiseVolume = 0.05f;
    }
    else if (playerHP == 4) {
        noiseVolume = 0.3f;
    }
    else if (playerHP == 3) {
        noiseVolume = 0.45f;
    }
    else if (playerHP == 2) {
        noiseVolume = 0.6f;
    }
    else if (playerHP <= 1) {
        noiseVolume = 0.8f;
    }

    AudioManager::GetInstance()->PlayOneShot(
        "Noise",
        noiseVolume
    );
    
    blocks.clear();


    // ========================================
    // ノイズの数
    // ========================================

    int blockCount = 4 + (std::rand() % 5);

    if (playerHP <= 4) blockCount += 3;
    if (playerHP <= 3) blockCount += 4;
    if (playerHP <= 2) blockCount += 5;
    if (playerHP <= 1) blockCount += 8;

    // ========================================
    // ノイズブロック生成
    // ========================================

    for (int i = 0; i < blockCount; i++) {

        GlitchBlock block;

        // HPが低いほど大きなノイズも発生
        if (playerHP <= 1 && i % 3 == 0) {
            block.width = 100 + (std::rand() % 250);
            block.height = 8 + (std::rand() % 18);
        }
        else if (playerHP <= 2 && i % 2 == 0) {
            block.width = 70 + (std::rand() % 170);
            block.height = 5 + (std::rand() % 12);
        }
        else {
            block.width = 18 + (std::rand() % 105);
            block.height = 2 + (std::rand() % 9);
        }

        block.width = (std::min)(block.width, screenWidth);

        block.x = std::rand() % (screenWidth - block.width + 1);
        block.y = std::rand() % screenHeight;

        // ========================================
        // ノイズの色
        // ========================================

        int colorType = std::rand() % 5;

        if (playerHP <= 1) {

            if (colorType <= 1) {
                block.color = GetColor(230, 35, 125);
            }
            else if (colorType == 2) {
                block.color = GetColor(150, 35, 220);
            }
            else if (colorType == 3) {
                block.color = GetColor(70, 35, 180);
            }
            else {
                block.color = GetColor(220, 210, 240);
            }

        }
        else if (playerHP <= 2) {

            if (colorType <= 1) {
                block.color = GetColor(180, 55, 210);
            }
            else if (colorType == 2) {
                block.color = GetColor(100, 65, 190);
            }
            else if (colorType == 3) {
                block.color = GetColor(100, 145, 210);
            }
            else {
                block.color = GetColor(210, 190, 230);
            }

        }
        else {

            if (colorType == 0) {
                block.color = GetColor(145, 105, 190);
            }
            else if (colorType == 1) {
                block.color = GetColor(105, 145, 190);
            }
            else if (colorType == 2) {
                block.color = GetColor(180, 100, 190);
            }
            else {
                block.color = GetColor(205, 205, 215);
            }
        }

        // ========================================
        // HPによる不透明度
        // ========================================

        block.alpha = 30 + (std::rand() % 31);

        if (playerHP <= 4) block.alpha += 10;
        if (playerHP <= 3) block.alpha += 10;
        if (playerHP <= 2) block.alpha += 15;
        if (playerHP <= 1) block.alpha += 20;

        if (block.alpha > 150) {
            block.alpha = 150;
        }

        blocks.push_back(block);
    }

    // ========================================
    // 横帯の発生
    // ========================================

    int bandChance = 3;

    if (playerHP <= 4) bandChance = 2;
    if (playerHP <= 2) bandChance = 1;

    if ((std::rand() % bandChance) == 0) {

        GlitchBlock band;

        band.x = 0;
        band.y = std::rand() % screenHeight;
        band.width = screenWidth;
        band.height = 2 + (std::rand() % 5);

        band.color = (playerHP <= 1)
            ? GetColor(230, 35, 125)
            : GetColor(160, 80, 210);

        band.alpha = 35 + (std::rand() % 25);

        if (playerHP <= 3) band.alpha += 15;
        if (playerHP <= 1) band.alpha += 25;

        if (band.alpha > 150) {
            band.alpha = 150;
        }

        blocks.push_back(band);
    }

    // ========================================
    // HP1：画面を横断する大型ノイズ
    // ========================================

    if (playerHP <= 1 && (std::rand() % 2 == 0)) {

        GlitchBlock largeBand;

        largeBand.x = 0;
        largeBand.y = std::rand() % screenHeight;
        largeBand.width = screenWidth;
        largeBand.height = 10 + (std::rand() % 25);
        largeBand.color = GetColor(180, 25, 150);
        largeBand.alpha = 55 + (std::rand() % 35);

        blocks.push_back(largeBand);
    }
}


// ========================================
// 死亡ノイズ開始
// ========================================

void NoiseEffect::StartDeath() {

    if (deathActive) {
        return;
    }

    deathActive = true;
    deathFrame = 0;

    blocks.clear();
    glitchDuration = 0;
}

// ========================================
// 死亡ノイズ更新
// ========================================

void NoiseEffect::UpdateDeath() {

    if (!deathActive) {
        return;
    }

    if (deathFrame < DEATH_DURATION) {
        deathFrame++;
    }
}

// ========================================
// 死亡ノイズ終了判定
// ========================================



void NoiseEffect::DrawDeath() {

    if (!deathActive) {
        return;
    }

    // 画面全体を暗い紫色で覆う
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);

    DrawBox(
        0, 0,
        screenWidth, screenHeight,
        GetColor(25, 0, 45),
        TRUE
    );

    // 画面全体にランダムなノイズを表示
    for (int i = 0; i < 500; i++) {

        int x = std::rand() % screenWidth;
        int y = std::rand() % screenHeight;

        int width = 5 + std::rand() % 100;
        int height = 2 + std::rand() % 15;

        int colorType = std::rand() % 4;

        int color;

        if (colorType == 0) {
            color = GetColor(230, 30, 160);
        }
        else if (colorType == 1) {
            color = GetColor(130, 30, 230);
        }
        else if (colorType == 2) {
            color = GetColor(220, 200, 255);
        }
        else {
            color = GetColor(10, 0, 25);
        }

        SetDrawBlendMode(
            DX_BLENDMODE_ALPHA,
            100 + std::rand() % 156
        );

        DrawBox(
            x, y,
            x + width, y + height,
            color,
            TRUE
        );
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool NoiseEffect::IsDeathFinished() const {

    return deathActive && deathFrame >= DEATH_DURATION;
}


// ========================================
// 描画処理
// ========================================

void NoiseEffect::Draw() {

    if (glitchDuration <= 0 || blocks.empty()) {
        return;
    }

    for (int i = 0; i < (int)blocks.size(); i++) {

        const GlitchBlock& block = blocks[i];

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, block.alpha);

        DrawBox(
            block.x,
            block.y,
            block.x + block.width,
            block.y + block.height,
            block.color,
            TRUE
        );
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
