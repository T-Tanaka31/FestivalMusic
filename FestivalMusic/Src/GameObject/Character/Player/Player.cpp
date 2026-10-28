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

#include <algorithm>
#include <cstdio>

// ========================================
// コンストラクタ
// ========================================

Player::Player(VECTOR _pos, std::string _tag)
	: Character(_pos, _tag)
	, jumpPower(14.3f)
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
	, attackCollider(nullptr)
	, jumpHoldFrames(-1)
	, horizontalVelocity(0.0f) {
	moveSpeed = 5.0f;
	Start();
}

// ========================================
// デストラクタ
// ========================================

Player::~Player() {
}

// ========================================
// 初期化
// ========================================

void Player::Start() {
	// プレイヤーの当たり判定
	SetCollider(new SquareCollider(
		this,
		VGet(50, 50, 0)
	));

	// 攻撃の当たり判定
	attackCollider = new SquareCollider(
		this,
		VGet(100, 60, 0)
	);

	attackCollider->SetTrigger(true);
	attackCollider->SetEnable(false);

	// プレイヤー画像：11列 × 7行
	LoadDivGraph(
		"Res/Player_fixed_11frames.png",
		77,
		11,
		7,
		128,
		128,
		images
	);

	// 攻撃エフェクト
	LoadDivGraph(
		"Res/Player_attack_effect.png",
		11,
		11,
		1,
		128,
		128,
		attackEffectImages
	);

	// HP設定
	maxHp = 100;
	hp = maxHp;

	if (hpBar == nullptr) {
		hpBar = new Gauge(
			hp,
			maxHp,
			hpBarPosX,
			hpBarPosY,
			hpBarWidth,
			hpBarHeight
		);
	}
}

// ========================================
// 更新処理
// ========================================

void Player::Update() {
	// ========================================
	// 入力取得
	// ========================================

	float stickX = input->IsJoypadSthick("L_Horizontal");		

	// アナログスティックの値を -1.0 ～ 1.0 に制限
	if (stickX < -1.0f) {
		stickX = -1.0f;
	}
	else if (stickX > 1.0f) {
		stickX = 1.0f;
	}
	// キーボード・十字キー
	bool digitalLeft =
		input->IsKey(KEY_INPUT_A) ||
		input->IsButton(XINPUT_GAMEPAD_DPAD_LEFT);

	bool digitalRight =
		input->IsKey(KEY_INPUT_D) ||
		input->IsButton(XINPUT_GAMEPAD_DPAD_RIGHT);

	// スティックの微小な入力を無視する
	const float DEAD_ZONE = 0.15f;

	bool stickLeft = stickX < -DEAD_ZONE;
	bool stickRight = stickX > DEAD_ZONE;

	// 左右移動判定
	bool moveLeft;
	bool moveRight;

	// キーボード・十字キーの入力を優先する
	if (digitalLeft || digitalRight) {
		moveLeft = digitalLeft && !digitalRight;
		moveRight = digitalRight && !digitalLeft;
	}
	else {
		moveLeft = stickLeft;
		moveRight = stickRight;
	}

	// ジャンプを押した瞬間
	bool jumpDown =
		input->IsKeyDown(KEY_INPUT_SPACE) ||
		input->IsButtonDown(XINPUT_GAMEPAD_A);

	// ジャンプを押している間
	bool jumpHeld =
		input->IsKey(KEY_INPUT_SPACE) ||
		input->IsButton(XINPUT_GAMEPAD_A);

	// 攻撃を押した瞬間
	bool attackDown =
		input->IsKeyDown(KEY_INPUT_J) ||
		input->IsButtonDown(XINPUT_GAMEPAD_X);

	// ========================================
	// 接地状態
	// ========================================

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
	// 攻撃開始
	// ========================================

	if (attackDown &&
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
		// 空中攻撃中は横方向の速度を維持する
		if (!isGround) {
			position.x += horizontalVelocity;
		}

		// 攻撃アニメーション
		attackTimer++;
		animTimer++;

		if (animTimer >= 4) {
			animTimer = 0;
			frame++;

			if (frame >= 11) {
				frame = 0;
				isAttacking = false;
				attackHit = false;

				animState = AnimState::Idle;
				animTimer = 0;
			}
		}
	}
	else {
		// ========================================
		// 移動速度の計算
		// ========================================

		if (digitalLeft || digitalRight) {
			// キーボード・十字キーは一定速度
			if (digitalLeft && !digitalRight) {
				horizontalVelocity = -moveSpeed;
				isRight = false;
			}
			else if (digitalRight && !digitalLeft) {
				horizontalVelocity = moveSpeed;
				isRight = true;
			}
			else {
				horizontalVelocity = 0.0f;
			}
		}
		else {
			// アナログスティックは倒した量に応じて速度を変える
			if (stickX < -DEAD_ZONE) {
				horizontalVelocity = moveSpeed * stickX;
				isRight = false;
			}
			else if (stickX > DEAD_ZONE) {
				horizontalVelocity = moveSpeed * stickX;
				isRight = true;
			}
			else {
				horizontalVelocity = 0.0f;
			}
		}

		// 横移動
		position.x += horizontalVelocity;

		// ========================================
		// ジャンプ開始
		// ========================================

		if (jumpDown && isGround) {
			velocity.y = -jumpPower;
			isGround = false;
			jumpHoldFrames = 0;
		}

		// ========================================
		// 5段階ジャンプ
		// ========================================

		if (jumpHoldFrames >= 0) {
			if (jumpHeld) {
				jumpHoldFrames++;
			}
			else {
				float jumpRatio = 1.0f;

				if (jumpHoldFrames <= 8) {
					jumpRatio = 0.5f;
				}
				else if (jumpHoldFrames <= 16) {
					jumpRatio = 0.60f;
				}
				else if (jumpHoldFrames <= 24) {
					jumpRatio = 0.80f;
				}
				else {
					jumpRatio = 1.0f;
				}

				// 上昇中だけジャンプ速度を調整
				if (velocity.y < 0.0f) {
					float targetVelocity =
						-jumpPower * jumpRatio;

					if (velocity.y < targetVelocity) {
						velocity.y = targetVelocity;
					}
				}

				jumpHoldFrames = -1;
			}
		}

		// ========================================
		// アニメーション状態
		// ========================================

		AnimState newState;

		if (velocity.y != 0.0f) {
			newState = AnimState::Jump;
		}
		else if (moveLeft != moveRight) {
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

			if (frame >= 11) {
				frame = 0;
			}
		}
	}

	// ========================================
	// 重力・落下
	// ========================================

	velocity.y += gravity;
	position.y += velocity.y;

	// ========================================
	// 攻撃判定
	// ========================================

	if (isAttacking && frame >= 4 && frame <= 7) {
		attackCollider->SetEnable(true);

		if (isRight) {
			attackCollider->SetOffset(VGet(75, 0, 0));
		}
		else {
			attackCollider->SetOffset(VGet(-75, 0, 0));
		}
	}
	else {
		attackCollider->SetEnable(false);
	}

	attackCollider->Update();

	// ========================================
	// マップ下への落下判定
	// ========================================

	float mapBottom = (float)MapLoader::GetMapBottom();

	if (position.y > mapBottom + 100.0f) {
		TakeDamage(hp);
		return;
	}

	// ========================================
	// Collider更新
	// ========================================

	pCollider->Update();

	// ========================================
	// デバッグ用HP減少
	// ========================================

#if _DEBUG
	if (input->IsKeyDown(KEY_INPUT_2)) {
		Damage(this, 10 + def);
	}

	// ========================================
	// HP回復
	// ========================================

	if (input->IsButtonDown(XINPUT_GAMEPAD_Y) ||
		input->IsKeyDown(KEY_INPUT_1)) {
		AddHp(maxHp / 10);
	}
#endif
}

// ========================================
// 描画処理
// ========================================

void Player::Render() {
	VECTOR camPos = Camera::main->GetPosition();

	int drawX =
		(int)(position.x - camPos.x + WINDOW_WIDTH / 2);

	int drawY =
		(int)(position.y - camPos.y + WINDOW_HEIGHT / 2);

	// ========================================
	// アニメーション画像選択
	// ========================================

	int imageIndex = 0;

	switch (animState) {
	case AnimState::Idle:
		imageIndex = frame;
		break;

	case AnimState::Walk:
		imageIndex = 22 + frame;
		break;

	case AnimState::Jump:
		imageIndex = 33 + frame;
		break;

	case AnimState::Attack:
		imageIndex = 44 + frame;
		break;
	}

	// ========================================
	// 攻撃エフェクト
	// ========================================

	if (isAttacking && frame >= 4 && frame <= 7) {
		int effectFrame = frame - 4;

		VECTOR effectPos = position;

		if (isRight) {
			effectPos.x += 60;
		}
		else {
			effectPos.x -= 60;
		}

		int effectX =
			(int)(effectPos.x - camPos.x + WINDOW_WIDTH / 2);

		int effectY =
			(int)(effectPos.y - camPos.y + WINDOW_HEIGHT / 2);

		DrawRotaGraph3(
			effectX,
			effectY,
			64,
			64,
			0.5,
			0.5,
			0.0,
			attackEffectImages[effectFrame],
			TRUE,
			isRight ? FALSE : TRUE,
			FALSE
		);
	}

	// ========================================
	// プレイヤー描画
	// ========================================

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

#if _DEBUG
	DrawString(
		drawX - 20,
		drawY - 40,
		tag.c_str(),
		COLOR_AMETHYST
	);
#endif

	pCollider->Render();

	if (attackCollider->IsEnable()) {
		attackCollider->Render();
	}

	// HPゲージ枠
	DrawBox(
		hpBarPosX - 3,
		hpBarPosY - 3,
		hpBarPosX + hpBarWidth + 3,
		hpBarPosY + hpBarHeight + 3,
		COLOR_BLACK,
		true
	);

	hpBar->Render();
}

// ========================================
// ダメージ処理
// ========================================

void Player::TakeDamage(int damage) {
	hp -= damage;

	if (hp <= 0) {
		hp = 0;

		SceneManager::GetInstance()->ChangeResultScene(
			ResultScene::ResultType::GameOver
		);
	}

#if _DEBUG
	printf("Player HP = %d\n", hp);
#endif
}

// ========================================
// Trigger開始
// ========================================

void Player::OnTriggerEnter(Collider* _pOther) {
}

// ========================================
// Trigger継続
// ========================================

void Player::OnTriggerStay(Collider* other) {
	// ゴール判定
	// ゴール判定
	if (other->GetGameObject()->GetTag() == "Goal") {
		SetGoalReached();
		return;
	}

	// ========================================
	// 敵への攻撃
	// ========================================

	if (other->GetGameObject()->GetTag() == "Enemy") {
		if (!isAttacking || attackHit) {
			return;
		}

		Enemy* enemy =
			dynamic_cast<Enemy*>(other->GetGameObject());

#if _DEBUG
		printfDx(
			"Tag = %s / EnemyPtr = %p\n",
			other->GetGameObject()->GetTag().c_str(),
			enemy
		);
#endif

		if (enemy != nullptr) {
#if _DEBUG
			printfDx("TakeDamage CALL\n");
#endif
			enemy->TakeDamage(5);
			attackHit = true;
		}
	}

	// ========================================
	// ブロックとの接地判定
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
			jumpHoldFrames = -1;
		}
	}
}

// ========================================
// Trigger終了
// ========================================

void Player::OnTriggerExit(Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() == "Block") {
		isGround = false;
	}
}