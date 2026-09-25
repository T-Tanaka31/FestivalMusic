#include "../../../Difinition/Colors.h"
#include "Enemy.h"
#include "../../../Component/Collider.h"
#include "../../../Difinition/Constant.h"
#include "../../Camera/Camera.h"

Enemy::Enemy(VECTOR _pos, std::string _tag)
	: Character(_pos, _tag)
	, isGround(false)
	, isHit(false) {
	Start();
}

Enemy::~Enemy() {
}

void Enemy::Start() {
	SetCollider(new SquareCollider(
		this,
		VGet(50, 50, 0)
	));

	maxHp = 10;
	hp = maxHp;
}

void Enemy::Update() {
	//	重力
	velocity.y += gravity;

	//	垂直移動
	position.y += velocity.y;

	pCollider->Update();
}

void Enemy::Render() {
	VECTOR camPos = Camera::main->GetPosition();

	int drawX = (int)(position.x - camPos.x + WINDOW_WIDTH / 2);
	int drawY = (int)(position.y - camPos.y + WINDOW_HEIGHT / 2);

	DrawCircle(
		drawX,
		drawY,
		20,
		COLOR_SKYBLUE,
		TRUE
	);

	DrawString(
		drawX - 20,
		drawY - 40,
		tag.c_str(),
		COLOR_CHARCOAL
	);

	pCollider->Render();
}

void Enemy::OnTriggerEnter(Collider* _pOther) {

}

void Enemy::OnTriggerStay(Collider* _pOther) {
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

		// 上から落下していて
		// めり込み量が小さい場合のみ接地
		if (velocity.y >= 0.0f &&
			overlapTop >= 0.0f &&
			overlapTop < 5.0f) {
			isGround = true;
			velocity.y = 0.0f;
		}
	}
}

void Enemy::OnTriggerExit(Collider* _pOther) {

}