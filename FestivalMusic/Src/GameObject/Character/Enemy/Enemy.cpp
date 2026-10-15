#include "../Player/Player.h"
#include "../../../Difinition/Colors.h"
#include "Enemy.h"
#include "../../../Component/Collider.h"
#include "../../../Difinition/Constant.h"
#include "../../Camera/Camera.h"
#include "../../../Map/MapLoader.h"

#include <cmath>
#include <cstdio>

// ========================================
// コンストラクタ
// ========================================
Enemy::Enemy(VECTOR _pos, std::string _tag)
    : Character(_pos, _tag)
    , isHit(false)
    , isGround(false)
    , moveSpeed(2.0f)
    , jumpPower(10.0f)
    , animState(EnemyAnimState::Idle)
    , frame(0)
    , animTimer(0)
    , isRight(true)
    , player(nullptr)
    , isFlying(false)
    , flyCenterY(0.0f)
    , flyAmplitude(30.0f)
    , flyTimer(0.0f)
    , flySpeed(0.05f)
    , isDying(false)
    , deathFinished(false)
    , deathFrame(0)
    , deathAnimTimer(0)
    , normalImagesLoaded(false)
    , flyImagesLoaded(false)
    , flyUpperLimitY(0.0f)
    , hasFlyUpperLimit(false) {
    Start();
}

// ========================================
// デストラクタ
// ========================================
Enemy::~Enemy() {
    // 通常敵の画像を解放
    if (normalImagesLoaded) {
        for (int i = 0; i < 77; i++) {
            if (images[i] != -1) {
                DeleteGraph(images[i]);
            }
        }
    }

    // ノイズカラスの画像を解放
    if (flyImagesLoaded) {
        for (int i = 0; i < 66; i++) {
            if (flyImages[i] != -1) {
                DeleteGraph(flyImages[i]);
            }
        }
    }
}

// ========================================
// 初期化
// ========================================
void Enemy::Start() {
    // 画像ハンドルを初期化
    for (int i = 0; i < 77; i++) {
        images[i] = -1;
    }

    for (int i = 0; i < 66; i++) {
        flyImages[i] = -1;
    }

    // 当たり判定
    SetCollider(new SquareCollider(
        this,
        VGet(50, 50, 0)
    ));

    // ------------------------------------
    // 通常敵の画像
    // ------------------------------------
    if (LoadDivGraph(
        "Res/Enemy_same_size_128x128_11frames.png",
        77,
        11,
        7,
        128,
        128,
        images
    ) != -1) {
        normalImagesLoaded = true;
    }
    else {
#if _DEBUG
        printf("通常敵の画像読み込み失敗\n");
#endif
    }

    // ------------------------------------
    // ノイズカラスの画像
    //
    // 1573 / 11 = 143
    // 1000 / 6 = 166.66...
    //
    // 高さ166で切り出すため、下端の
    // 4ピクセルは使用しない。
    // ------------------------------------
    if (LoadDivGraph(
        "Res/NoiseCrow_1573x1000.png",
        66,
        11,
        6,
        143,
        166,
        flyImages
    ) != -1) {
        flyImagesLoaded = true;
    }
    else {
#if _DEBUG
        printf("ノイズカラスの画像読み込み失敗\n");
#endif
    }

    maxHp = 10;
    hp = maxHp;
}

// ========================================
// 飛行設定
// ========================================
void Enemy::SetFlying(float amplitude, float speed) {
    isFlying = true;

    flyCenterY = position.y;
    flyAmplitude = amplitude;
    flySpeed = speed;
    flyTimer = 0.0f;

    velocity.y = 0.0f;
    isGround = false;

    animState = EnemyAnimState::Fly;

    frame = 0;
    animTimer = 0;

    flyAttackState = FlyAttackState::Hover;
    diveTimer = 0;
    attackCooldownTimer = 0;
    attackHasHit = false;
}

// ========================================
// 更新
// ========================================
void Enemy::Update() {
    // ------------------------------------
    // 死亡アニメーション
    // ------------------------------------
    if (isDying) {
        deathAnimTimer++;

        if (deathAnimTimer >= 6) {
            deathAnimTimer = 0;

            if (deathFrame < 10) {
                deathFrame++;
            }
            else {
                deathFinished = true;
            }
        }

        return;
    }

    // ------------------------------------
    // 飛行敵
    // ------------------------------------
    // ------------------------------------
// 飛行敵（ノイズ烏）
// ------------------------------------
    if (isFlying) {
        if (attackCooldownTimer > 0) {
            attackCooldownTimer--;
        }

        switch (flyAttackState) {
        case FlyAttackState::Hover:
        {
            // 通常の上下浮遊
            flyTimer += flySpeed;

            position.y =
                flyCenterY +
                std::sin(flyTimer) * flyAmplitude;

            velocity.y = 0.0f;
            animState = EnemyAnimState::Fly;

            // プレイヤーが近づいたら攻撃開始
            if (player != nullptr &&
                attackCooldownTimer <= 0) {

                VECTOR playerPos = player->GetPosition();

                float dx = playerPos.x - position.x;
                float dy = playerPos.y - position.y;

                float distance =
                    std::sqrt(dx * dx + dy * dy);

                if (distance <= 250.0f && distance > 0.0f) {
                    diveTargetX = playerPos.x;
                    diveTargetY = playerPos.y;

                    float length = std::sqrt(
                        dx * dx + dy * dy
                    );

                    diveVelocityX = dx / length * 7.0f;
                    diveVelocityY = dy / length * 7.0f;

                    isRight = (dx <= 0.0f);

                    flyAttackState = FlyAttackState::Dive;
                    diveTimer = 0;
                    attackHasHit = false;

                    frame = 0;
                    animTimer = 0;
                }
            }
            break;
        }

        case FlyAttackState::Dive:
        {
            // プレイヤーを狙って急降下
            position.x += diveVelocityX;
            position.y += diveVelocityY;

            diveTimer++;

            // 目標地点に近づいたら、または一定時間で復帰
            float dx = diveTargetX - position.x;
            float dy = diveTargetY - position.y;

            if (std::sqrt(dx * dx + dy * dy) < 20.0f ||
                diveTimer >= 45) {

                flyAttackState = FlyAttackState::Recover;
                velocity.y = 0.0f;
            }
            break;
        }

        case FlyAttackState::Recover:
        {
            // 元の高さまで上昇
            float dy = flyCenterY - position.y;

            if (std::fabs(dy) <= 5.0f) {
                position.y = flyCenterY;

                flyAttackState = FlyAttackState::Hover;
                attackCooldownTimer = 90;

                flyTimer = 0.0f;
            }
            else {
                position.y += (dy > 0.0f) ? 5.0f : -5.0f;
            }
            break;
        }
        }

        // 飛行敵は重力を使わない
        velocity.y = 0.0f;
    }
    else {
        // --------------------------------
        // 通常敵のプレイヤー追跡
        // --------------------------------
        if (player != nullptr) {
            float distanceX =
                player->GetPosition().x - position.x;

            if (distanceX > 0.0f) {
                position.x += moveSpeed;
                isRight = true;
                animState = EnemyAnimState::Walk;
            }
            else if (distanceX < 0.0f) {
                position.x -= moveSpeed;
                isRight = false;
                animState = EnemyAnimState::Walk;
            }
            else {
                animState = EnemyAnimState::Idle;
            }
        }

        // --------------------------------
        // 通常敵の重力
        // --------------------------------
        if (!isGround) {
            animState = EnemyAnimState::Jump;
        }

        velocity.y += gravity;
        position.y += velocity.y;
    }

    // ------------------------------------
    // Collider更新
    // ------------------------------------
    if (pCollider != nullptr) {
        pCollider->Update();
    }

    // ------------------------------------
    // アニメーション更新
    // ------------------------------------
    animTimer++;

    if (animTimer >= 6) {
        animTimer = 0;
        frame++;

        if (frame >= 11) {
            frame = 0;
        }
    }
}

// ========================================
// 描画
// ========================================
void Enemy::Render() {
    VECTOR camPos = Camera::main->GetPosition();

    int drawX =
        (int)(position.x - camPos.x + WINDOW_WIDTH / 2);

    int drawY =
        (int)(position.y - camPos.y + WINDOW_HEIGHT / 2);

    int imageIndex = 0;
    int imageHandle = -1;

    // ------------------------------------
    // 飛行敵（ノイズカラス）
    // ------------------------------------
    if (isFlying) {
        if (!flyImagesLoaded) {
            return;
        }

        if (isDying) {
            // 5行目：死亡アニメーション
            imageIndex = 44 + deathFrame;
        }
        else {
            switch (flyAttackState) {
            case FlyAttackState::Hover:
                position.y = flyCenterY
                    + std::sin(flyTimer) * flyAmplitude;

                if (hasFlyUpperLimit &&
                    position.y < flyUpperLimitY) {
                    position.y = flyUpperLimitY;
                }

                // 1行目：通常飛行
                imageIndex = frame;

                // プレイヤーのいる方向を向く
                if (player != nullptr) {
                    float dx = player->GetPosition().x - position.x;

                    if (std::fabs(dx) > 1.0f) {
                        isRight = (dx < 0.0f);
                    }
                }
                break;

            case FlyAttackState::Dive:
                // 3行目：横向きの飛行モーション
                imageIndex = 22 + frame;
                break;

            case FlyAttackState::Recover:
                // 4行目：復帰時の飛行モーション
                imageIndex = 33 + frame;
                break;
            }
        }

        if (imageIndex < 0 || imageIndex >= 66) {
            return;
        }

        imageHandle = flyImages[imageIndex];

        if (imageHandle != -1) {
            DrawRotaGraph3(
                drawX,
                drawY,
                71,
                83,
                0.5,
                0.5,
                0.0,
                imageHandle,
                TRUE,
                isRight,
                FALSE
            );
        }
    }
    // ------------------------------------
    // 通常敵
    // ------------------------------------
    else {
        if (!normalImagesLoaded) {
            return;
        }

        if (isDying) {
            // 7行目：死亡アニメーション
            imageIndex = 55 + deathFrame;
        }
        else {
            switch (animState) {
            case EnemyAnimState::Idle:
                imageIndex = frame;
                break;

            case EnemyAnimState::Walk:
                imageIndex = 22 + frame;
                break;

            case EnemyAnimState::Jump:
                imageIndex = 33 + frame;
                break;

            case EnemyAnimState::Fly:
                imageIndex = frame;
                break;
            }
        }

        if (imageIndex < 0 || imageIndex >= 77) {
            return;
        }

        imageHandle = images[imageIndex];

        if (imageHandle != -1) {
            DrawRotaGraph3(
                drawX,
                drawY,
                64,
                64,
                0.5,
                0.5,
                0.0,
                imageHandle,
                TRUE,
                isRight ? FALSE : TRUE,
                FALSE
            );
        }
    }

    // ------------------------------------
    // 当たり判定のデバッグ表示
    // ------------------------------------
#if _DEBUG
    if (pCollider != nullptr) {
        pCollider->Render();
    }
#endif
}

// ========================================
// ダメージ
// ========================================
void Enemy::TakeDamage(int damage) {
    if (isDying) {
        return;
    }

    hp -= damage;

    if (hp <= 0) {
        hp = 0;

        isDying = true;
        deathFinished = false;

        deathFrame = 0;
        deathAnimTimer = 0;

        if (pCollider != nullptr) {
            pCollider->SetEnable(false);
        }

#if _DEBUG
        printf("Enemy死亡アニメーション開始\n");
#endif
    }

#if _DEBUG
    printf("敵HP = %d\n", hp);
#endif
}

// ========================================
// 衝突開始
// ========================================
void Enemy::OnTriggerEnter(Collider* _pOther) {
}

// ========================================
// 衝突継続
// ========================================
void Enemy::OnTriggerStay(Collider* _pOther) {
    if (isDying || _pOther == nullptr) {
        return;
    }

    // ------------------------------------
    // ノイズ烏の攻撃判定
    // ------------------------------------
    if (isFlying) {
        if (_pOther->GetGameObject()->GetTag() == "Player") {
            // 急降下中だけダメージを与える
            if (flyAttackState == FlyAttackState::Dive &&
                !attackHasHit) {

                Player* target =
                    dynamic_cast<Player*>(
                        _pOther->GetGameObject()
                    );

                if (target != nullptr) {
                    target->TakeDamage(10);
                    attackHasHit = true;
                }
            }
        }


        // 飛行敵は通常の地上衝突処理を行わない
        return;
    }

    // ------------------------------------
    // 通常敵のブロック衝突処理
    // ------------------------------------
    if (_pOther->GetGameObject()->GetTag() == "Block") {
        SquareCollider* myCol =
            dynamic_cast<SquareCollider*>(pCollider);

        SquareCollider* blockCol =
            dynamic_cast<SquareCollider*>(_pOther);

        if (!myCol || !blockCol) {
            return;
        }

        float overlapTop =
            myCol->GetMaxPoint().y -
            blockCol->GetMinPoint().y;

        if (velocity.y >= 0.0f &&
            overlapTop >= 0.0f &&
            overlapTop < 5.0f) {

            isGround = true;
            velocity.y = 0.0f;
        }
    }
}
// ========================================
// 衝突終了
// ========================================
void Enemy::OnTriggerExit(Collider* _pOther) {
    if (!isFlying) {
        isGround = false;
    }
}

// ========================================
// 死亡判定
// ========================================
bool Enemy::IsDead() const {
    if (isDying) {
        return deathFinished;
    }

    if (position.y > MapLoader::GetMapBottom() + 100.0f) {
        return true;
    }

    return false;
}

// ========================================
// 死亡中かどうか
// ========================================
bool Enemy::IsDying() const {
    return isDying;
}