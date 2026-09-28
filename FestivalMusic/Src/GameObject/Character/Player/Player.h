#pragma once
#include "../../../Manager/InputManager.h"
#include "../Character.h"
#include "../../../UI/Gauge.h"
#include "../../../Component/Collider.h"

class Enemy;

enum class AnimState {
	Idle,
	Walk,
	Jump,
	Attack,
	Run,
	Damage,
	Victory
};

class Player : public Character {
private:

	// ジャンプ力
	float jumpPower;

	// 地面に触れているか
	bool isGround;

	// 入力マネージャー
	InputManager* input;

	const float GROUND_Y = 1000.0f;

	bool isHit = false;

	int images[84];

	AnimState animState;

	int frame;
	int animTimer;

	// 向いている方向
	bool isRight;

	// 攻撃関連
private:
	bool isAttacking;
	bool attackHit;
	int attackTimer;
	int attackCooldown;

	// 攻撃用Collider
	SquareCollider* attackCollider;


#pragma region ゲージ関連

	Gauge<int>* hpBar;

	int hpBarPosX = 145;
	int hpBarPosY = 100;
	int hpBarWidth = 300;
	int hpBarHeight = 25;

	int uX = 100;
	int uY = 100;
	int lX = 100;
	int lY = 70;

#pragma endregion


public:

	// コンストラクタ
	Player(
		VECTOR _pos = VZero,
		std::string _tag = "Player"
	);

	// デストラクタ
	~Player();

	void Start() override;

	void Update() override;

	void Render() override;


public:

	void SetGround(bool value) {
		isGround = value;
	}

	void TakeDamage(int damage);


	// ==============================
	// 攻撃関連
	// ==============================

	bool IsAttacking() const {
		return isAttacking;
	}

	bool IsRight() const {
		return isRight;
	}

	int GetAttackFrame() const {
		return frame;
	}

	bool CanAttackHit() const {
		return !attackHit;
	}

	void SetAttackHit() {
		attackHit = true;
	}


public:

	// ==============================
	// 衝突判定
	// ==============================

	// 入ったとき
	void OnTriggerEnter(
		Collider* _pOther
	) override;

	// 入っているとき
	void OnTriggerStay(
		Collider* _pOther
	) override;

	// 出たとき
	void OnTriggerExit(
		Collider* _pOther
	) override;
};