#include "../../../Manager/InputManager.h"
#include "../../../Difinition/Colors.h"
#include "Player.h"
#include "../../../Component/Collider.h"
#include "../../../Difinition/Constant.h"
#include "../../Camera/Camera.h"

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
		VGet(50, 50, 0)
	));
	
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

	// 重力
	velocity.y += gravity;

	// 垂直移動
	position.y += velocity.y;

	// 仮地面
	if (position.y >= GROUND_Y) {
		position.y = GROUND_Y;
		velocity.y = 0.0f;
		isGround = true;
	}

	pCollider->Update();
}


void Player::Render() {
	VECTOR camPos = Camera::main->GetPosition();

	int drawX = (int)(position.x - camPos.x + WINDOW_WIDTH / 2);
	int drawY = (int)(position.y - camPos.y + WINDOW_HEIGHT / 2);

	DrawCircle(
		drawX,
		drawY,
		20,
		COLOR_MAROON,
		TRUE
	);

	DrawString(
		drawX - 20,
		drawY - 40,
		tag.c_str(),
		COLOR_AMETHYST
	);

	pCollider->Render();
}
void Player::OnTriggerEnter(Collider* _pOther) {

}

void Player::OnTriggerStay(Collider* other) {
	if (other->GetGameObject()->GetTag() != "Block") {
		return;
	}

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


void Player::OnTriggerExit(Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() == "Block") {
		isGround = false;
	}
}