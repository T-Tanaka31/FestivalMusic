#pragma once
#include "../Character.h"

class Collider;
class Player;

enum class EnemyAnimState {
    Idle,
    Walk,
    Jump,
    Fly
};

class Enemy : public Character {
private:
    bool isHit;
    bool isGround;

    float moveSpeed;
    float jumpPower;

    EnemyAnimState animState;

    int frame;
    int animTimer;

    // 通常の敵：11列×7行
    int images[77];

    // ノイズカラス：11列×6行
    int flyImages[66];

    bool isRight;

    // 追跡するPlayer
    Player* player;

    // 飛行敵用
    bool isFlying;

    float flyCenterY;
    float flyAmplitude;
    float flyTimer;
    float flySpeed;

    float flyUpperLimitY;  // 飛行できる上限のY座標
    bool hasFlyUpperLimit; // 上限が設定されているか

    // 死亡アニメーション
    bool isDying;
    bool deathFinished;

    int deathFrame;
    int deathAnimTimer;

    // 画像読み込み状態
    bool normalImagesLoaded;
    bool flyImagesLoaded;

    // ノイズ烏の攻撃状態
    enum class FlyAttackState {
        Hover,      // 通常飛行
        Dive,       // 急降下攻撃
        Recover     // 元の高さへ復帰
    };

    FlyAttackState flyAttackState = FlyAttackState::Hover;

    float diveTargetX = 0.0f;
    float diveTargetY = 0.0f;

    float diveVelocityX = 0.0f;
    float diveVelocityY = 0.0f;

    int diveTimer = 0;
    int attackCooldownTimer = 0;

    bool attackHasHit = false;

public:
    Enemy(
        VECTOR _pos = VZero,
        std::string _tag = "Enemy"
    );

    ~Enemy();

    void Start() override;
    void Update() override;
    void Render() override;

    // 飛行モードを有効にする
    void SetFlying(
        float amplitude = 30.0f,
        float speed = 0.05f
    );

    void SetPlayer(Player* _player) {
        player = _player;
    }

    void SetGround(bool value) {
        isGround = value;
    }

    void TakeDamage(int damage);

    bool IsDead() const;
    bool IsDying() const;

    bool IsFlying() const {
        return isFlying;
    }

    VECTOR GetVelocity() const {
        return velocity;
    }

    void SetPosition(VECTOR pos) {
        position = pos;
    }

    void SetVelocityY(float y) {
        velocity.y = y;
    }


    void OnTriggerEnter(Collider* _pOther) override;
    void OnTriggerStay(Collider* _pOther) override;
    void OnTriggerExit(Collider* _pOther) override;
};