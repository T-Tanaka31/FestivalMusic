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
}

Player::~Player() {
}

void Player::Start() {
	//	初期化処理
	
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
}