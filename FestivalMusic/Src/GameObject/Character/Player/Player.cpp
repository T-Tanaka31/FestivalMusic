#include "../../../Manager/InputManager.h"
#include "../../../Difinition/Colors.h"
#include "Player.h"
#include "../Enemy/Enemy.h"
#include "../../../Component/Collider.h"
#include "../../../Difinition/Constant.h"
#include "../../Camera/Camera.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Map/MapLoader.h"
#include "../../../Scene/ResultScene.h"

Player::Player(VECTOR _pos, std::string _tag)
	: Character(_pos, _tag)
	, jumpPower(10.0f)
	, isGround(false)
	, input(InputManager::GetInstance())
	, animState(AnimState::Idle)
	, frame(0)
	, animTimer(0)
	, isRight(true)
	, isAttacking(false)
	, attackHit(false)
	, attackTimer(0)
	, attackCooldown(0)
	, attackCollider(nullptr) {
	moveSpeed = 5.0f;
	Start();
}
Player::~Player() {

}

void Player::Start() {

	SetCollider(new SquareCollider(
		this,
		VGet(50, 50, 0)
	));

	// 攻撃用Collider
	attackCollider = new SquareCollider(
		this,
		VGet(100, 60, 0)
	);

	// 攻撃Colliderは押し返しを発生させない
	attackCollider->SetTrigger(true);

	attackCollider->SetEnable(false);


	LoadDivGraph(
		"Res/Player_fixed_11frames.png",
		77,
		11,
		7,
		128,
		128,
		images
	);

	maxHp = 100;
	hp = maxHp;

	if (hpBar == nullptr)
		hpBar = new Gauge(
			hp,
			maxHp,
			hpBarPosX,
			hpBarPosY,
			hpBarWidth,
			hpBarHeight
		);
}

void Player::Update() {

	if (velocity.y != 0.0f) {
		isGround = false;
	}

	// ========================================
	// 攻撃クールタイム
	// ========================================

	if (attackCooldown > 0) {
		attackCooldown--;
	}


	// ========================================
	// 攻撃
	// ========================================

	if (input->IsKeyDown(KEY_INPUT_J) &&
		attackCooldown <= 0 &&
		!isAttacking) {

		isAttacking = true;
		attackHit = false;

		attackTimer = 0;
		frame = 0;
		animTimer = 0;

		animState = AnimState::Attack;

		attackCooldown = 20;
	}


	// ========================================
	// 攻撃中
	// ========================================

	if (isAttacking) {

		attackTimer++;

		// 攻撃アニメーション
		animTimer++;

		if (animTimer >= 4) {
			animTimer = 0;

			frame++;

			// 12フレーム再生したら終了
			if (frame >= 12) {
				frame = 0;
				isAttacking = false;
				attackHit = false;
				animState = AnimState::Idle;
				animTimer = 0;
			}
		}

		// 攻撃中は移動させない
	}
	else {

		// ========================================
		// 左移動
		// ========================================

		if (input->IsKey(KEY_INPUT_A)) {
			position.x -= moveSpeed;
			isRight = false;
		}


		// ========================================
		// 右移動
		// ========================================

		if (input->IsKey(KEY_INPUT_D)) {
			position.x += moveSpeed;
			isRight = true;
		}


		// ========================================
		// ジャンプ
		// ========================================

		if (input->IsKeyDown(KEY_INPUT_SPACE) && isGround) {

			if (input->IsKey(KEY_INPUT_W)) {
				velocity.y = -jumpPower * 1.4f;
			}
			else if (input->IsKey(KEY_INPUT_S)) {
				velocity.y = -jumpPower / 1.4f;
			}
			else {
				velocity.y = -jumpPower;
			}

			isGround = false;
		}


		// ========================================
		// アニメーション状態
		// ========================================

		AnimState newState;

		if (velocity.y != 0.0f) {
			newState = AnimState::Jump;
		}
		else if (
			input->IsKey(KEY_INPUT_A) ||
			input->IsKey(KEY_INPUT_D)) {

			newState = AnimState::Walk;
		}
		else {
			newState = AnimState::Idle;
		}


		if (animState != newState) {

			animState = newState;

			frame = 0;
			animTimer = 0;
		}


		// 通常アニメーション
		animTimer++;

		if (animTimer >= 6) {

			animTimer = 0;

			frame++;

			if (frame >= 12) {
				frame = 0;
			}
		}
	}


	// ========================================
	// 重力
	// ========================================

	velocity.y += gravity;

	position.y += velocity.y;

	// ========================================
	// 攻撃Collider
	// ========================================

	if (isAttacking &&
		frame >= 4 &&
		frame <= 7) {

		attackCollider->SetEnable(true);

		if (isRight) {
			attackCollider->SetOffset(
				VGet(75, 0, 0)
			);
		}
		else {
			attackCollider->SetOffset(
				VGet(-75, 0, 0)
			);
		}
	}
	else {
		attackCollider->SetEnable(false);
	}

	attackCollider->Update();

	// マップの下に落ちたら死亡
	float mapBottom = (float)MapLoader::GetMapBottom();

	if (position.y > mapBottom + 100.0f) {
		TakeDamage(hp);
		return;
	}

	// ========================================
	// Collider
	// ========================================

	pCollider->Update();


	// ========================================
	// その他
	// ========================================

	if (input->IsKeyDown(KEY_INPUT_2)) {
		Damage(this, 10 + def);
	}

	if (
		input->IsButtonDown(XINPUT_GAMEPAD_Y) ||
		input->IsKeyDown(KEY_INPUT_1)) {

		AddHp(maxHp / 10);
	}
}
void Player::Render() {
	VECTOR camPos = Camera::main->GetPosition();

	int drawX =
		(int)(position.x - camPos.x + WINDOW_WIDTH / 2);

	int drawY =
		(int)(position.y - camPos.y + WINDOW_HEIGHT / 2);


	int imageIndex = 0;

	switch (animState) {
	case AnimState::Idle:
		// 1行目
		imageIndex = frame;
		break;

	case AnimState::Walk:
		// 3行目
		imageIndex = 24 + frame;
		break;

	case AnimState::Jump:
		// 4行目
		imageIndex = 36 + frame;
		break;

	case AnimState::Attack:
		// 5行目
		imageIndex = 48 + frame;
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


	DrawString(
		drawX - 20,
		drawY - 40,
		tag.c_str(),
		COLOR_AMETHYST
	);


	pCollider->Render();


	DrawBox(
		hpBarPosX - 3,
		hpBarPosY - 3,
		hpBarPosX + hpBarWidth + 3,
		hpBarPosY + hpBarHeight + 3,
		COLOR_BLACK,
		true
	);

	if (attackCollider->IsEnable()) {
		attackCollider->Render();
	}

	hpBar->Render();
}

void Player::TakeDamage(int damage) {
	hp -= damage;

	if (hp <= 0) {
		hp = 0;

		SceneManager::GetInstance()->ChangeResultScene(
			ResultScene::ResultType::GameOver
		);
	}

	printf("Player HP = %d\n", hp);
}


void Player::OnTriggerEnter(Collider* _pOther) {

}
void Player::OnTriggerStay(Collider* other) {
	// ========================================
	// Goal
	// ========================================

	if (other->GetGameObject()->GetTag() == "Goal") {

		SceneManager::GetInstance()->ChangeResultScene(
			ResultScene::ResultType::GameClear
		);

		return;
	}


	// ========================================
	// Enemy
	// ========================================

	if (other->GetGameObject()->GetTag() == "Enemy") {

		if (!isAttacking) {
			return;
		}

		if (attackHit) {
			return;
		}

		Enemy* enemy =
			dynamic_cast<Enemy*>(
				other->GetGameObject()
			);

		printfDx(
			"Tag = %s / EnemyPtr = %p\n",
			other->GetGameObject()->GetTag().c_str(),
			enemy
		);

		if (enemy != nullptr) {

			printfDx("TakeDamage CALL\n");

			enemy->TakeDamage(5);

			attackHit = true;
		}

	}



	// ========================================
	// Block
	// ========================================

	if (other->GetGameObject()->GetTag() == "Block") {

		SquareCollider* myCol =
			dynamic_cast<SquareCollider*>(pCollider);

		SquareCollider* blockCol =
			dynamic_cast<SquareCollider*>(other);

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
void Player::OnTriggerExit(Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() == "Block") {
		isGround = false;
	}
}
