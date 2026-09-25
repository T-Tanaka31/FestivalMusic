#include "../Player/Player.h"
#include "../../../Difinition/Colors.h"
#include "Enemy.h"
#include "../../../Component/Collider.h"
#include "../../../Difinition/Constant.h"
#include "../../Camera/Camera.h"

Enemy::Enemy(VECTOR _pos, std::string _tag)
	: Character(_pos, _tag)
	, isGround(false)
	, isHit(false)
	, moveSpeed(2.0f)
	, jumpPower(10.0f)
	, animState(EnemyAnimState::Idle)
	, frame(0)
	, animTimer(0)
	, isRight(true)
	, player(nullptr) {
	Start();
}

Enemy::~Enemy() {
}

void Enemy::Start() {
	SetCollider(new SquareCollider(
		this,
		VGet(50, 50, 0)
	));

	LoadDivGraph(
		"Res/Enemy_same_size_128x128_11frames.png",
		77,
		11,
		7,
		128,
		128,
		images
	);

	maxHp = 10;
	hp = maxHp;
}
void Enemy::Update() {
	if (player != nullptr) {
		float distanceX =
			player->GetPosition().x - position.x;

		float stopDistance = 80.0f;

		// プレイヤーが右側
		if (distanceX > stopDistance) {
			position.x += moveSpeed;

			isRight = true;
			animState = EnemyAnimState::Walk;
		}
		// プレイヤーが左側
		else if (distanceX < -stopDistance) {
			position.x -= moveSpeed;

			isRight = false;
			animState = EnemyAnimState::Walk;
		}
		// 近すぎる
		else {
			animState = EnemyAnimState::Idle;
		}
	}

	// 空中ならジャンプアニメーション
	if (!isGround) {
		animState = EnemyAnimState::Jump;
	}

	// 重力
	velocity.y += gravity;

	position.y += velocity.y;

	// Collider更新
	pCollider->Update();

	// アニメーション
	animTimer++;

	if (animTimer >= 6) {
		animTimer = 0;

		frame++;

		if (frame >= 11) {
			frame = 0;
		}
	}
}

void Enemy::Render() {
	VECTOR camPos = Camera::main->GetPosition();

	int drawX =
		(int)(position.x - camPos.x + WINDOW_WIDTH / 2);

	int drawY =
		(int)(position.y - camPos.y + WINDOW_HEIGHT / 2);

	int imageIndex = 0;

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
	}

	DrawRotaGraph3(
		drawX,
		drawY,
		64,
		64,
		0.5,
		0.5,
		0.0,
		images[imageIndex],
		TRUE,
		isRight ? FALSE : TRUE,
		FALSE
	);

	pCollider->Render();
}

void Enemy::TakeDamage(int damage) {
	hp -= damage;

	if (hp <= 0) {
		hp = 0;

		if (pCollider != nullptr) {
			pCollider->SetEnable(false);
		}

		printf("Enemy死亡\n");
	}

	printf("敵HP = %d\n", hp);
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