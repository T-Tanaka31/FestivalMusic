#include "../../../Manager/InputManager.h"
#include "../../../Difinition/Colors.h"
#include "Player.h"
#include "../Enemy/Enemy.h"
#include "../../../Component/Collider.h"
#include "../../../Difinition/Constant.h"
#include "../../Camera/Camera.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/AudioManager.h"
#include "../../../Map/MapLoader.h"
#include "../../MapObject/MovingBlock.h"

#include <algorithm>
#include <cstdio>
#include <cmath>

int Player::remainingLives = 99;
int Player::killCount = 0;
bool Player::preserveLivesOnNextScene = false;


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
	, horizontalVelocity(0.0f)
	, invincibleTimer(0)
	, stageRestartRequested(false)
	, neonStarTimer(0)
	, attackType(AttackType::Normal)
	, downAttackInvincibleTimer(0)
	, isDownAttackLanding(false) {
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

	// 上攻撃・下攻撃専用スプライト：11列 × 8行
	LoadDivGraph(
		"Res/Player.png",
		88,
		11,
		8,
		128,
		128,
		dedicatedAttackImages
	);

	// HPはハート5個分
	maxHp = 5;
	hp = maxHp;

}

// ========================================
// 更新処理
// ========================================

void Player::Update() {
	// 無敵時間のカウントダウン
	if (invincibleTimer > 0) {
		invincibleTimer--;
	}

	// 急降下攻撃の無敵時間カウントダウン
	if (downAttackInvincibleTimer > 0) {
		downAttackInvincibleTimer--;
	}

	// ========================================
	// 移動ブロックに乗っている場合
	// ========================================

	if (ridingMovingBlock != nullptr && isGround) {

		VECTOR delta = ridingMovingBlock->GetFrameDelta();

		// ブロックと同じ距離だけプレイヤーを移動
		position = VAdd(position, delta);
	}
	// ========================================
	// 入力取得
	// ========================================

	// 左スティックの左右入力
	float stickX = input->IsJoypadSthick("L_Horizontal");

	// 左スティックの上下入力
	float stickY = input->IsJoypadSthick("L_Vertical");

	// アナログスティックの値を -1.0 ～ 1.0 に制限
	if (stickX < -1.0f) {
		stickX = -1.0f;
	}
	else if (stickX > 1.0f) {
		stickX = 1.0f;
	}

	if (stickY < -1.0f) {
		stickY = -1.0f;
	}
	else if (stickY > 1.0f) {
		stickY = 1.0f;
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
		input->IsButtonDown(XINPUT_GAMEPAD_A) ||
		input->IsButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER);

	// ジャンプを押している間
	bool jumpHeld =
		input->IsKey(KEY_INPUT_SPACE) ||
		input->IsButton(XINPUT_GAMEPAD_A) ||
		input->IsButton(XINPUT_GAMEPAD_LEFT_SHOULDER);

	// ========================================
	// 右スティック入力
	// ========================================

	float rightStickX =
		input->IsJoypadSthick("R_Horizontal");

	float rightStickY =
		input->IsJoypadSthick("R_Vertical");

	// 右スティックのデッドゾーン
	const float RIGHT_STICK_DEAD_ZONE = 0.5f;

	// 右スティックが倒されているか
	bool rightStickActive =
		(std::fabs(rightStickX) > RIGHT_STICK_DEAD_ZONE ||
		 std::fabs(rightStickY) > RIGHT_STICK_DEAD_ZONE);

	// 右スティックを倒した瞬間だけ攻撃する
	static bool rightStickWasActive = false;

	bool rightStickAttack =
		rightStickActive && !rightStickWasActive;

	rightStickWasActive = rightStickActive;


	// ========================================
	// 攻撃入力
	// ========================================

	bool keyboardAttack =
		input->IsKeyDown(KEY_INPUT_J) ||
		input->IsButtonDown(XINPUT_GAMEPAD_X);

	bool attackDown =
		keyboardAttack || rightStickAttack;


	// ========================================
	// 攻撃方向
	// ========================================

	bool upAttackInput = false;
	bool downAttackInput = false;

	// 右スティックを使った場合
	if (rightStickActive) {

		// 横と縦のどちらに強く倒しているか判定
		if (std::fabs(rightStickX) >
			std::fabs(rightStickY)) {

			// 横方向の通常攻撃
			if (rightStickX > RIGHT_STICK_DEAD_ZONE) {
				isRight = true;
			}
			else if (rightStickX < -RIGHT_STICK_DEAD_ZONE) {
				isRight = false;
			}
		}
		else {

			// 上攻撃
			if (rightStickY > RIGHT_STICK_DEAD_ZONE) {
				upAttackInput = true;
			}
			// 下攻撃
			else if (rightStickY < -RIGHT_STICK_DEAD_ZONE) {
				downAttackInput = true;
			}
		}
	}
	else {

		// キーボード・十字キー・左スティック
		upAttackInput =
			input->IsKey(KEY_INPUT_W) ||
			input->IsButton(XINPUT_GAMEPAD_DPAD_UP) ||
			stickY > DEAD_ZONE;

		downAttackInput =
			input->IsKey(KEY_INPUT_S) ||
			input->IsButton(XINPUT_GAMEPAD_DPAD_DOWN) ||
			stickY < -DEAD_ZONE;
	}

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
		!isAttacking &&
		!isDownAttackLanding) {

		isAttacking = true;
		attackHit = false;
		hitEnemies.clear();

		attackTimer = 0;
		frame = 0;
		animTimer = 0;

		// 攻撃方向を決定
		if (upAttackInput) {

			attackType = AttackType::Up;
		}
		else if (downAttackInput && !isGround) {

			attackType = AttackType::Down;

			// 急降下開始直後は無敵
			downAttackInvincibleTimer = 10;

			// 下方向へ急降下
			velocity.y = 8.0f;
			jumpHoldFrames = -1;
		}
		else {

			// 通常攻撃
			attackType = AttackType::Normal;
		}

		AudioManager::GetInstance()->PlayOneShot("Naruko");
		animState = AnimState::Attack;
		attackCooldown = 20;
	}

	// ========================================
	// 攻撃・移動・アニメーション
	// ========================================

	if (isAttacking) {

		if (!isGround) {
			position.x += horizontalVelocity;
		}

		animTimer++;

		if (animTimer >= 4) {
			animTimer = 0;

			if (attackType == AttackType::Down) {

				if (isDownAttackLanding) {
					// 着地モーション：82～87フレーム
					frame++;

					if (frame >= 11) {
						isAttacking = false;
						isDownAttackLanding = false;
						attackHit = false;
						frame = 0;
						animTimer = 0;
						animState = AnimState::Idle;
					}
				}
				else if (!isGround) {
					// 急降下開始アニメーション
					if (frame < 3) {
						frame++;
					}
					// 空中では80・81フレームをループ
					else if (frame == 3) {
						frame = 4;
					}
					else {
						frame = 3;
					}
				}
			}
			else {
				// 通常攻撃・上攻撃
				frame++;

				if (frame >= 11) {
					isAttacking = false;
					attackHit = false;
					frame = 0;
					animTimer = 0;
					animState = AnimState::Idle;
				}
			}
		}
	}
	else {
		// 移動速度
		if (digitalLeft && !digitalRight) {
			horizontalVelocity = -moveSpeed;
			isRight = false;
		}
		else if (digitalRight && !digitalLeft) {
			horizontalVelocity = moveSpeed;
			isRight = true;
		}
		else if (!digitalLeft && !digitalRight &&
			stickX < -DEAD_ZONE) {
			horizontalVelocity = moveSpeed * stickX;
			isRight = false;
		}
		else if (!digitalLeft && !digitalRight &&
			stickX > DEAD_ZONE) {
			horizontalVelocity = moveSpeed * stickX;
			isRight = true;
		}
		else {
			horizontalVelocity = 0.0f;
		}

		position.x += horizontalVelocity;

		// ジャンプ
		if (jumpDown && isGround) {
			ridingMovingBlock = nullptr;
			velocity.y = -jumpPower;
			isGround = false;
			jumpHoldFrames = 0;
		}

		// 5段階ジャンプ
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

				if (velocity.y < 0.0f) {
					float targetVelocity = -jumpPower * jumpRatio;

					if (velocity.y < targetVelocity) {
						velocity.y = targetVelocity;
					}
				}

				jumpHoldFrames = -1;
			}
		}

		// 通常アニメーション
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

		animTimer++;

		if (animTimer >= 6) {
			animTimer = 0;
			frame = (frame + 1) % 11;
		}
	}

	// ========================================
// 重力・落下
// ========================================

	if (isDownAttackLanding && isGround) {
		velocity.y = 0.0f;
	}
	else {
		if (isAttacking && attackType == AttackType::Down) {
			// 下攻撃中の重力
			velocity.y += gravity * 0.5f;

			// 下攻撃中の最大落下速度
			velocity.y = (std::min)(velocity.y, 30.0f);
		}
		else {
			// 通常時の重力
			velocity.y += gravity;

			// 通常時の最大落下速度
			velocity.y = (std::min)(velocity.y, 30.0f);
		}

		position.y += velocity.y;
	}

	// ========================================
// 攻撃判定
// ========================================

	bool canAttack = false;

	if (isAttacking) {

		if (attackType == AttackType::Down) {

			if (isDownAttackLanding) {
				// 着地モーションの最初の2フレーム
				canAttack = (frame >= 5 && frame <= 6);
			}
			else {
				// 急降下中は2フレーム目から
				canAttack = (frame >= 1);
			}
		}
		else {
			// 通常攻撃・上攻撃
			canAttack = (frame >= 4 && frame <= 7);
		}
	}

	if (canAttack) {

		attackCollider->SetEnable(true);

		switch (attackType) {

		case AttackType::Normal:
			if (isRight) {
				attackCollider->SetOffset(VGet(75, 0, 0));
			}
			else {
				attackCollider->SetOffset(VGet(-75, 0, 0));
			}
			break;

		case AttackType::Up:
			attackCollider->SetOffset(VGet(0, -50, 0));
			break;

		case AttackType::Down:
			if (isDownAttackLanding) {
				// 着地攻撃
				attackCollider->SetOffset(VGet(0, 10, 0));
			}
			else {
				// 急降下攻撃
				attackCollider->SetOffset(VGet(0, 10, 0));
			}
			break;
		}
	}
	else {
		attackCollider->SetEnable(false);
	}

	// ========================================
	// ステージ左右端の移動制限
	// ========================================

	float mapLeft = (float)MapLoader::GetMapLeft();
	float mapRight = (float)MapLoader::GetMapRight();

	// プレイヤー画像の横幅は約64pxなので半分の32px
	const float PLAYER_HALF_WIDTH = 32.0f;

	// 左端
	float minPlayerX = mapLeft + PLAYER_HALF_WIDTH;

	// 右端
	float maxPlayerX = mapRight - PLAYER_HALF_WIDTH;

	// 移動範囲を制限
	position.x = (std::max)(
		minPlayerX,
		(std::min)(position.x, maxPlayerX)
	);

	attackCollider->Update();

	// ========================================
	// マップ下への落下判定
	// ========================================

	float mapBottom = (float)MapLoader::GetMapBottom();

	if (position.y > mapBottom + 100.0f) {
		DieImmediately();
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

	// ネオンスターの効果時間
	if (neonStarTimer > 0) {
		neonStarTimer--;
	}

	// 1UP演出の時間
	if (oneUpTimer > 0) {
		oneUpTimer--;
	}
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

	// 通常は従来のスプライトを使用
	int imageHandle = images[imageIndex];

	// 上攻撃・下攻撃は専用スプライトを使用
	if (isAttacking) {
		if (attackType == AttackType::Up) {
			imageHandle = dedicatedAttackImages[66 + frame]; // 7行目
		}
		else if (attackType == AttackType::Down) {
			imageHandle = dedicatedAttackImages[77 + frame]; // 8行目
		}
	}

	// ========================================
	// 攻撃エフェクト
	// ========================================

	if (isAttacking &&
		attackType == AttackType::Normal &&
		frame >= 4 && frame <= 7) {
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
	// プレイヤー描画（ダメージ時の点滅）
	// ========================================

	// 無敵時間中は一定間隔で点滅させる
	if (invincibleTimer <= 0 || (invincibleTimer / 5) % 2 == 0) {

		if (neonStarTimer > 0) {
			int colorIndex = (neonStarTimer / 5) % 4;

			SetDrawBright(
				(colorIndex == 0 || colorIndex == 3) ? 255 : 180,
				(colorIndex == 0 || colorIndex == 2) ? 255 : 180,
				(colorIndex == 1 || colorIndex == 2) ? 255 : 180
			);

			DrawRotaGraph3(
				drawX, drawY,
				64, 64,
				0.5, 0.5,
				0.0,
				imageHandle,
				TRUE,
				isRight ? FALSE : TRUE,
				FALSE
			);

			SetDrawBright(255, 255, 255);
		}
		else {
			DrawRotaGraph3(
				drawX, drawY,
				64, 64,
				0.5, 0.5,
				0.0,
				imageHandle,
				TRUE,
				isRight ? FALSE : TRUE,
				FALSE
			);
		}
	}

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

	// ========================================
	// HUD：ライフ5個と残機（カメラに追従しない固定表示）
	// ========================================

	const int hudX = 28;
	const int hudY = 28;
	const int heartSize = 18;
	const int heartGap = 8;

	DrawString(hudX, hudY - 17, "LIFE", GetColor(255, 255, 255));

	// ハートを5個表示。残っているHPは赤、失ったHPは暗い灰色。
	for (int i = 0; i < maxHp; i++) {
		int x = hudX + i * (heartSize + heartGap);
		int y = hudY + 4;
		unsigned int heartColor =
			(i < hp) ? GetColor(240, 55, 75) : GetColor(75, 75, 85);

		// 2つの丸と三角形でハートを描画
		DrawCircle(x + 6, y + 5, 6, heartColor, TRUE);
		DrawCircle(x + 14, y + 5, 6, heartColor, TRUE);
		DrawTriangle(
			x, y + 7,
			x + 20, y + 7,
			x + 10, y + 21,
			heartColor, TRUE
		);
	}
	// ========================================
	// 残機表示（画面右上）
	// ========================================

	int livesX = WINDOW_WIDTH - 120;
	int livesY = 28;

	DrawString(
		livesX,
		livesY - 17,
		"REMAIN",
		GetColor(255, 255, 255)
	);

	DrawFormatString(
		livesX,
		livesY + 3,
		GetColor(255, 255, 255),
		"x %02d",
		remainingLives
	);

	// ========================================
	// 撃破数表示（残機表示の下）
	// ========================================

	const int killHudY = livesY + 42;
	const unsigned int killTextColor = GetColor(255, 225, 120);
	DrawFormatString(
		livesX - 48,
		killHudY,
		GetColor(255, 255, 255),
		"DEFEAT  %02d / 20",
		killCount
	);

	// 20体までの進捗バー
	const int barX = livesX - 48;
	const int barY = killHudY + 22;
	const int barW = 150;
	const int barH = 8;
	DrawBox(barX, barY, barX + barW, barY + barH,
		GetColor(65, 65, 75), TRUE);
	int progressW = barW * killCount / 20;
	if (progressW > 0) {
		DrawBox(barX, barY, barX + progressW, barY + barH,
			killTextColor, TRUE);
	}
	DrawBox(barX, barY, barX + barW, barY + barH,
		GetColor(210, 210, 220), FALSE);

	// ========================================
	// 1UP演出
	// ========================================

	if (oneUpTimer > 0) {
		// 中央に大きく1UP!を表示。点滅ではなく滑らかに消える。
		int alpha = 255;
		if (oneUpTimer < 20) {
			alpha = oneUpTimer * 255 / 20;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		const int centerX = WINDOW_WIDTH / 2;
		const int centerY = WINDOW_HEIGHT / 2 - 45;
		const unsigned int gold = GetColor(255, 215, 55);
		const unsigned int white = GetColor(255, 255, 220);

		// 星のような光線を周囲に描画
		int pulse = (oneUpTimer / 4) % 2;
		int spread = 48 + pulse * 8;
		DrawLine(centerX - spread, centerY - 12,
			centerX - spread - 18, centerY - 28, gold, 4);
		DrawLine(centerX + spread, centerY - 12,
			centerX + spread + 18, centerY - 28, gold, 4);
		DrawLine(centerX - spread, centerY + 12,
			centerX - spread - 18, centerY + 28, gold, 4);
		DrawLine(centerX + spread, centerY + 12,
			centerX + spread + 18, centerY + 28, gold, 4);
		DrawCircle(centerX - spread - 20, centerY, 5, white, TRUE);
		DrawCircle(centerX + spread + 20, centerY, 5, white, TRUE);

		// 文字の影と本体
		DrawString(centerX - 43 + 3, centerY - 18 + 3,
			"1UP!", GetColor(90, 45, 0));
		DrawString(centerX - 43, centerY - 18, "1UP!", gold);
		DrawString(centerX - 43, centerY + 12,
			"EXTRA LIFE", white);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


void Player::TakeDamage(int damage) {

	if (stageRestartRequested) {
		return;
	}

	if (invincibleTimer > 0 ||
		(isAttacking && attackType == AttackType::Down)) {
		return;
	}

	(void)damage;

	hp--;
	invincibleTimer = 60;

	if (hp <= 0) {
		hp = 0;
		killCount = 0;

		// 残機を減らす
		remainingLives--;

		if (remainingLives < 0) {
			remainingLives = 0;
		}

		// GameSceneに死亡演出を依頼する
		// ここではシーンを切り替えない
		stageRestartRequested = true;
	}

#if _DEBUG
	printf("Player HP = %d / Lives = %d\n",
		hp, remainingLives);
#endif
}


void Player::DieImmediately() {

	if (stageRestartRequested) {
		return;
	}

	hp = 0;
	killCount = 0;

	// 残機を減らす
	remainingLives--;

	if (remainingLives < 0) {
		remainingLives = 0;
	}

	// 即死でも同じ死亡演出を行う
	stageRestartRequested = true;

#if _DEBUG
	printf("即死！ Player HP = %d / Lives = %d\n",
		hp, remainingLives);
#endif
}

// ========================================
// 敵撃破数の加算
// ========================================
void Player::AddKillCount() {
	killCount++;

	// 20体撃破するごとに1UP
	while (killCount >= 20) {
		killCount -= 20;
		remainingLives++;
		oneUpTimer = 60; // 約1秒間表示（60FPS想定）

#if _DEBUG
		printf("1UP！ 残機 = %d\n", remainingLives);
#endif
	}

#if _DEBUG
	printf("敵撃破数 = %d / 20\n", killCount);
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
	if (other->GetGameObject()->GetTag() == "Goal") {
		SetGoalReached();
		return;
	}

	// ========================================
	// 敵への攻撃
	// ========================================

	if (other->GetGameObject()->GetTag() == "Enemy") {
		if (!isAttacking) {
			return;
		}

		Enemy* enemy =
			dynamic_cast<Enemy*>(other->GetGameObject());

		if (enemy == nullptr) {
			return;
		}

		// この攻撃中に、すでにダメージを与えた敵か確認する
		auto it = std::find(hitEnemies.begin(), hitEnemies.end(), enemy);
		if (it != hitEnemies.end()) {
			return;
		}

		// 敵ごとに1回だけダメージを与える
		hitEnemies.push_back(enemy);

#if _DEBUG
		printfDx("Enemy hit: %p\n", enemy);
#endif

		if (enemy->TakeDamage(5)) {
			// 倒した敵ごとに撃破数を加算
			AddKillCount();
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

			neonStarTimer = 60;

			// 急降下攻撃の着地モーション開始
			if (isAttacking &&
				attackType == AttackType::Down &&
				!isDownAttackLanding) {

				isDownAttackLanding = true;

				// 着地アニメーション開始
				frame = 5;
				animTimer = 0;

				horizontalVelocity = 0.0f;
				attackHit = false;
				hitEnemies.clear(); // 着地攻撃は別の攻撃判定として扱う

				// 着地直後の無敵時間
				downAttackInvincibleTimer = 10;

				// 着地した瞬間から攻撃判定を有効にする
				attackCollider->SetOffset(VGet(0, 30, 0));
				attackCollider->SetEnable(true);
			}

			// ========================================
			// 移動ブロック判定
			// ========================================

			ridingMovingBlock =
				dynamic_cast<MovingBlock*>(
					other->GetGameObject()
				);
		}
	}
}

// ========================================
// Trigger終了
// ========================================
void Player::OnTriggerExit(Collider* _pOther) {

	if (_pOther->GetGameObject()->GetTag() == "Block") {

		isGround = false;

		// 移動ブロックから離れたら解除
		if (ridingMovingBlock ==
			dynamic_cast<MovingBlock*>(
				_pOther->GetGameObject()
				)) {

			ridingMovingBlock = nullptr;
		}
	}
}