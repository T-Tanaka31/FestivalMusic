#pragma once
#include "../../../Manager/InputManager.h"
#include "../../GameObject.h"
class Player : public GameObject {
private:
	//	速度
	float moveSpeed;

	//	ジャンプ力
	float jumpPower;

	//	地面に触れているか
	bool isGround;

	//	入力マネージャー
	InputManager* input;

	//	重力加速度
	float gravity = 0.5f;

	const float GROUND_Y = 1000.0f;

	bool isHit = false;
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

