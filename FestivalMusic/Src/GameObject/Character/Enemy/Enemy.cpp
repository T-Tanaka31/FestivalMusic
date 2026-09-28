#include "../Player/Player.h"
#include "../../../Difinition/Colors.h"
#include "Enemy.h"
#include "../../../Component/Collider.h"
#include "../../../Difinition/Constant.h"
#include "../../Camera/Camera.h"
#include "../../../Map/MapLoader.h"

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
	, player(nullptr)
	, isDying(false)
	, deathFinished(false)
	, deathFrame(0)
	, deathAnimTimer(0) {
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
	66,
	11,
	6,
	128,
	128,
	images
	);

	maxHp = 10;
	hp = maxHp;
}
void Enemy::Update() {
	// =========================
	// 死亡アニメーション
	// =========================
	if (isDying) {
		deathAnimTimer++;

		if (deathAnimTimer >= 6) {
			deathAnimTimer = 0;

			// 0～10の11フレーム
			if (deathFrame < 10) {
				deathFrame++;
			}
			else {
				// 最後のフレームを表示し終わった
				deathFinished = true;
			}
		}

		return;
	}


	// =========================
	// プレイヤー追跡
	// =========================
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


	// =========================
	// 重力
	// =========================
	if (!isGround) {
		animState = EnemyAnimState::Jump;
	}

	velocity.y += gravity;
	position.y += velocity.y;


	// =========================
	// Collider更新
	// =========================
	if (pCollider != nullptr) {
		pCollider->Update();
	}


	// =========================
	// 通常アニメーション
	// =========================
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

	// =========================
	// 死亡アニメーション
	// =========================
	if (isDying) {
		imageIndex = 55 + deathFrame;

		if (imageIndex > 65) {
			imageIndex = 65;
		}
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
		}
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

	if (pCollider != nullptr) {
		pCollider->Render();
	}
}
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

		printf("Enemy死亡アニメーション開始\n");
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

bool Enemy::IsDead() const {
	if (isDying) {
		return deathFinished;
	}

	if (position.y > MapLoader::GetMapBottom() + 100.0f) {
		return true;
	}

	return false;
}

bool Enemy::IsDying() const {
	return isDying;
}