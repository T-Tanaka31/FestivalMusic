
#pragma once
#include <vector>

class NoiseEffect {
private:
    struct GlitchBlock {
        int x;
        int y;
        int width;
        int height;
        int color;
        int alpha;
    };

    std::vector<GlitchBlock> blocks;

    int elapsedFrames;
    int nextGlitchFrame;
    int glitchDuration;
    int screenWidth;
    int screenHeight;

    // 死亡時の全画面ノイズ
    bool deathActive;
    int deathFrame;
    static const int DEATH_DURATION = 120;

public:
    NoiseEffect();

    // 通常ノイズ
    void Update(int stage, int playerHP);
    void Draw();

    // 死亡ノイズ
    void StartDeath();
    void UpdateDeath();
    bool IsDeathFinished() const;
    void DrawDeath();
};
