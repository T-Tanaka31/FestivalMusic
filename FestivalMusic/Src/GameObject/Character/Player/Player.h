#pragma once
#include "../../../Manager/InputManager.h"
#include "../Character.h"
#include "../../../Component/Collider.h"
#include <vector>

class Enemy;
class MovingBlock;

enum class AnimState {
	Idle,
	Walk,
	Jump,
	Attack,
	Run,
	Damage,
	Victory
};

// 攻撃の種類
enum class AttackType {
	Normal,
	Up,
	Down
};

class Player : public Character {
private:
	float jumpPower;
	int jumpHoldFrames;
	float horizontalVelocity;
	bool isGround;
	bool isGoalReached = false;
	InputManager* input;
	const float GROUND_Y = 1000.0f;
	bool isHit = false;
	AttackType attackType;
	AnimState animState;
	int images[84];
	int frame;
	int animTimer;
	bool isRight;
	int invincibleTimer;

	static int remainingLives;
	static bool preserveLivesOnNextScene;
	bool stageRestartRequested;
	static int killCount;
	int oneUpTimer = 0;
	int neonStarTimer = 0;
	MovingBlock* ridingMovingBlock = nullptr;

private:
	bool isAttacking;
	bool attackHit; // 互換性のため保持。敵ごとの判定にはhitEnemiesを使用
	int attackTimer;
	int attackCooldown;
	int attackEffectImages[11];
	int downAttackInvincibleTimer;
	bool isDownAttackLanding;
	int dedicatedAttackImages[88];
	SquareCollider* attackCollider;

	// 現在の攻撃でダメージを与えた敵。敵ごとに1回だけ当てる
	std::vector<Enemy*> hitEnemies;

public:
	Player(VECTOR _pos = VZero, std::string _tag = "Player");
	~Player();
	void Start() override;
	void Update() override;
	void Render() override;

	// 現在のHPを取得
	int GetHP() const {
		return hp;
	}

	void SetGround(bool value) { isGround = value; }
	void TakeDamage(int damage);
	void DieImmediately();

	static void ResetLives() {
		remainingLives = 3;
		killCount = 0;
	}
	static int GetRemainingLives() { return remainingLives; }
	static void PreserveLivesForStageRestart() { preserveLivesOnNextScene = true; }
	static bool ConsumePreserveLivesFlag() {
		bool value = preserveLivesOnNextScene;
		preserveLivesOnNextScene = false;
		return value;
	}
	bool ConsumeStageRestartRequest() {
		bool value = stageRestartRequested;
		stageRestartRequested = false;
		return value;
	}

	void AddKillCount();
	static int GetKillCount() { return killCount; }

	bool IsGoalReached() const { return isGoalReached; }
	void SetGoalReached() { isGoalReached = true; }

	bool IsAttacking() const { return isAttacking; }
	bool IsRight() const { return isRight; }
	int GetAttackFrame() const { return frame; }

	// 攻撃全体ではなく敵ごとに命中を管理するため、攻撃中は常に判定可能
	bool CanAttackHit() const { return isAttacking; }
	void SetAttackHit() { attackHit = true; }

	void OnTriggerEnter(Collider* _pOther) override;
	void OnTriggerStay(Collider* _pOther) override;
	void OnTriggerExit(Collider* _pOther) override;
};
