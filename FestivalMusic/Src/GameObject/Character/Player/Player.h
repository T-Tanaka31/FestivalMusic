#pragma once
#include "../../../Manager/InputManager.h"
#include "../Character.h"
#include "../../../UI/Gauge.h"

enum class AnimState {
	Idle,
	Walk,
	Run,
	Jump,
	Attack,
	Damage,
	Victory
};

class Player : public Character {
private:

	//	ジャンプ力
	float jumpPower;

	//	地面に触れているか
	bool isGround;

	//	入力マネージャー
	InputManager* input;

	const float GROUND_Y = 1000.0f;

	bool isHit = false;

	int images[84];

	AnimState animState;

	int frame;
	int animTimer;

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
	//	コンストラクタ
	Player(VECTOR _pos = VZero, std::string _tag = "Player");

	//	デストラクタ
	~Player();

	void Start() override;

	void Update() override;

	void Render() override;

public:
	void SetGround(bool value) {
		isGround = value;
	}

public:	//	オーバーライドした衝突判定
	/// <summary>
	/// 入ったとき
	/// </summary>
	/// <param name="_pOther"></param>
	void OnTriggerEnter(Collider* _pOther) override;

	/// <summary>
	/// 入っているとき
	/// </summary>
	/// <param name="_pOther"></param>
	void OnTriggerStay(Collider* _pOther) override;

	/// <summary>
	/// 出たとき
	/// </summary>
	/// <param name="_pOther"></param>
	void OnTriggerExit(Collider* _pOther) override;
};

