
#pragma once

#include "../../Player/Player.h"
#include "../Enemy.h"

class Player;

// ボスの行動状態
enum class BossState {
    Approach,   // 接近
    Windup,     // 攻撃準備
    Attack,     // 攻撃
    Recovery    // 攻撃後の隙
};

class Boss : public Enemy {
private:
    Player* bossTarget;

    BossState bossState;

    int stateTimer;

    float bossMoveSpeed;

    int contactDamage;
    bool attackHasHit;

    void UpdateBossAI();

public:
    Boss(VECTOR pos = VZero);

    ~Boss() override = default;

    void Start() override;
    void Update() override;
    void Render() override;

    void SetTarget(Player* target);

    void OnTriggerStay(Collider* other) override;

    BossState GetBossState() const {
        return bossState;
    }
};
