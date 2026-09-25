#pragma once
#include "../Character.h"
class Enemy : public Character {
private:
	bool isHit;

	//	地面に触れているか
	bool isGround;
public:
	//	コンストラクタ
	Enemy(VECTOR _pos = VZero, std::string _tag = "Enemy");

	//	デストラクタ
	~Enemy();

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
	/// 入ってるとき
	/// </summary>
	/// <param name="_pOther"></param>
	void OnTriggerStay(Collider* _pOther) override;

	/// <summary>
	/// 出たとき
	/// </summary>
	/// <param name="_pOther"></param>
	void OnTriggerExit(Collider* _pOther) override;
};


