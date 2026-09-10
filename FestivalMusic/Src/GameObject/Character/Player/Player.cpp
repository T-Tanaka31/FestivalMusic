#include "../../../Manager/InputManager.h"
#include "../../../Difinition/Colors.h"
#include "Player.h"
#include "../../../Component/Collider.h"

Player::Player(VECTOR _pos, std::string _tag)
	: GameObject(_pos, _tag)
	, moveSpeed(5.0f)
	, jumpPower(10.0f)
	, isGround(false)
	, input(InputManager::GetInstance()) {
	Start();
}

Player::~Player() {
}

void Player::Start() {
	SetCollider(new SquareCollider(
		this,
		VGet(50,50,0)
	));
}

void Player::Update() {
	//	更新処理
	//	入力処理
	if (input->IsKey(KEY_INPUT_A)) {
		position.x -= moveSpeed;
	}
	if (input->IsKey(KEY_INPUT_D)) {
		position.x += moveSpeed;
	}

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

	velocity.y += gravity;
	position.y += velocity.y;

	if (position.y >= GROUND_Y) {
		position.y = GROUND_Y;
		velocity.y = 0.0f;
		isGround = true;
	}

	pCollider->Update();

	pCollider->Update();

	// テスト用エリア
	VECTOR testMin = VGet(1000, 1000, 0);
	VECTOR testMax = VGet(1200, 1200, 0);

	// PlayerのCollider取得
	SquareCollider* col = dynamic_cast<SquareCollider*>(pCollider);

	if (col) {
		isHit =
			col->GetMinPoint().x < testMax.x &&
			col->GetMaxPoint().x > testMin.x &&
			col->GetMinPoint().y < testMax.y &&
			col->GetMaxPoint().y > testMin.y;
	}
}

void Player::Render() {
	DrawCircle(
		(int)position.x,
		(int)position.y,
		20,
		COLOR_MAROON,
		TRUE
	);

	DrawString(
		(int)position.x - 20,
		(int)position.y - 40,
		tag.c_str(),
		COLOR_AMETHYST
	);

	// テスト用矩形
	DrawBox(
		1000,
		1000,
		1200,
		1200,
		COLOR_BLUE,
		FALSE
	);

	if (isHit) {
		DrawString(
			10,
			10,
			"HIT!",
			COLOR_RED
		);
	}

	pCollider->Render();
}