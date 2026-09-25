#include "../../../Manager/InputManager.h"
#include "../../../Difinition/Colors.h"
#include "Player.h"
#include "../../../Component/Collider.h"
#include "../../../Difinition/Constant.h"
#include "../../Camera/Camera.h"
#include "../../../Manager/SceneManager.h"

Player::Player(VECTOR _pos, std::string _tag)
	: Character(_pos, _tag)
	, jumpPower(10.0f)
	, isGround(false)
	, input(InputManager::GetInstance())
	, animState(AnimState::Idle)
	, frame(0)
	, animTimer(0) {
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

	LoadDivGraph(
	"Res/Player_fixed_11frames.png",
	77,     // 11 × 7
	11,     // 横11
	7,      // 縦7
	128,
	128,
	images
	);

	maxHp = 100;
	hp = maxHp;

	if (hpBar == nullptr)
		hpBar = new Gauge(hp, maxHp, hpBarPosX, hpBarPosY, hpBarWidth, hpBarHeight);

}

void Player::Update() {
	// 左移動
	if (input->IsKey(KEY_INPUT_A)) {
		position.x -= moveSpeed;
	}

	// 右移動
	if (input->IsKey(KEY_INPUT_D)) {
		position.x += moveSpeed;
	}

	// ジャンプ
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

	// --------------------------------
	// アニメーション状態を決定
	// --------------------------------

	AnimState newState;

	if (!isGround) {
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

	// アニメーションが切り替わったら
	// 最初のフレームから再生
	if (animState != newState) {
		animState = newState;
		frame = 0;
		animTimer = 0;
	}

	// --------------------------------
	// アニメーション更新
	// --------------------------------

	animTimer++;
	if (animTimer >= 6) {
		animTimer = 0;

		frame++;

		if (frame >= 11) {
			frame = 0;
		}
	}

	// --------------------------------
	// 重力
	// --------------------------------

	velocity.y += gravity;

	// 垂直移動
	position.y += velocity.y;

	// Collider更新
	pCollider->Update();

	// --------------------------------
	// HP処理
	// --------------------------------

	if (input->IsKeyDown(KEY_INPUT_2)) {
		Damage(this, 10 + def);
	}

	if (input->IsButtonDown(XINPUT_GAMEPAD_Y) ||
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

	// --------------------------------
	// アニメーション画像番号
	// --------------------------------

	int imageIndex = 0;

	switch (animState) {

	case AnimState::Idle:
		imageIndex = frame;          // 0～10
		break;

	case AnimState::Walk:
		imageIndex = 22 + frame;     // 3行目
		break;

	case AnimState::Jump:
		imageIndex = 33 + frame;     // 4行目
		break;
	}

	// --------------------------------
	// キャラクター描画
	// --------------------------------

	DrawRotaGraph(
		drawX,
		drawY,
		0.5,
		0.0,
		images[imageIndex],
		TRUE
	);

	DrawString(
		drawX - 20,
		drawY - 40,
		tag.c_str(),
		COLOR_AMETHYST
	);

	// Collider
	pCollider->Render();

	// HPバー
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
void Player::OnTriggerEnter(Collider* _pOther) {

}

void Player::OnTriggerStay(Collider* other) {

	if (other->GetGameObject()->GetTag() == "Goal") {
		SceneManager::GetInstance()->ChangeScene(SceneType::Result);
	}

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


void Player::OnTriggerExit(Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() == "Block") {
		isGround = false;
	}
}