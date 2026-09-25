#pragma once
#include "../Character.h"

class Collider;
class Player;

enum class EnemyAnimState {
	Idle,
	Walk,
	Jump
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

	int images[77];

	bool isRight;

	// 追跡するPlayer
	Player* player;

public:
	Enemy(VECTOR _pos = VZero, std::string _tag = "Enemy");

	~Enemy();

	void Start() override;

	void Update() override;

	void Render() override;

	// Playerを設定
	void SetPlayer(Player* _player) {
		player = _player;
	}

public:
	void SetGround(bool value) {
		isGround = value;
	}

	void TakeDamage(int damage);
public:
	void OnTriggerEnter(Collider* _pOther) override;

	void OnTriggerStay(Collider* _pOther) override;

	void OnTriggerExit(Collider* _pOther) override;
};