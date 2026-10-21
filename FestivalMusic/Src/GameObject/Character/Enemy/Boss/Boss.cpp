#include "Boss.h"

#include "../../../../Component/Collider.h"

#include <cmath>

Boss::Boss(VECTOR pos)
    : Enemy(pos, "Enemy")
    , bossTarget(nullptr)
    , bossState(BossState::Approach)
    , stateTimer(0)
    , bossMoveSpeed(1.5f)
    , contactDamage(15)
    , attackHasHit(false) {
    // Enemyの初期化が終わった後にボスのHPを設定
    maxHp = 1000;
    hp = maxHp;

    atk = contactDamage;
}

void Boss::Start() {
    // 親クラスのStartはEnemyのコンストラクタで実行済み。
    // ここで再実行すると画像や当たり判定を二重に作るため、
    // 意図的に呼び出さない。
}

void Boss::SetTarget(Player* target) {
    bossTarget = target;

    // 親クラスの追跡処理でも同じプレイヤーを使う
    SetPlayer(target);
}

void Boss::Update() {
    // 既存の移動・重力・当たり判定・アニメーションを利用
    Enemy::Update();

    if (IsDying() || IsDead() || bossTarget == nullptr) {
        return;
    }

    UpdateBossAI();
}

void Boss::UpdateBossAI() {
    if (stateTimer > 0) {
        stateTimer--;
    }

    switch (bossState) {
    case BossState::Approach:
    {
        VECTOR targetPos = bossTarget->GetPosition();

        float dx = targetPos.x - position.x;
        float dy = targetPos.y - position.y;

        // 十分近づいたら攻撃準備へ
        if (std::fabs(dx) < 100.0f &&
            std::fabs(dy) < 100.0f) {
            bossState = BossState::Windup;
            stateTimer = 45;
            attackHasHit = false;
        }

        break;
    }

    case BossState::Windup:
    {
        // 攻撃前の予備動作
        if (stateTimer <= 0) {
            bossState = BossState::Attack;
            stateTimer = 20;
            attackHasHit = false;
        }

        break;
    }

    case BossState::Attack:
    {
        // この時間中にプレイヤーへ接触するとダメージ
        if (stateTimer <= 0) {
            bossState = BossState::Recovery;
            stateTimer = 60;
        }

        break;
    }

    case BossState::Recovery:
    {
        // 攻撃後の隙が終わったら再び接近
        if (stateTimer <= 0) {
            bossState = BossState::Approach;
        }

        break;
    }
    }
}

void Boss::Render() {
    // 仮素材として既存の敵スプライトを使用
    Enemy::Render();
}

void Boss::OnTriggerStay(Collider* other) {
    if (other == nullptr || IsDying()) {
        return;
    }

    // 親クラスのブロック衝突処理などを維持
    Enemy::OnTriggerStay(other);

    if (bossState != BossState::Attack || attackHasHit) {
        return;
    }

    if (other->GetGameObject()->GetTag() != "Player") {
        return;
    }

    Player* target =
        dynamic_cast<Player*>(other->GetGameObject());

    if (target != nullptr) {
        target->TakeDamage(contactDamage);
        attackHasHit = true;
    }
}
