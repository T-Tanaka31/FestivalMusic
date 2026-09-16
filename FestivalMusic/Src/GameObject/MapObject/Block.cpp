#include "Block.h"
#include "../Camera/Camera.h"
#include "../../Component/Collider.h"
#include "../../Difinition/Constant.h"
#include "../../Difinition/Colors.h"

Block::Block(VECTOR _pos, VECTOR _size)
	: GameObject(_pos, "Block")
	, size(_size) {
	pCollider = new SquareCollider(this, size);
}

Block::~Block() {
}

void Block::Start() {
}

void Block::Update() {
	pCollider->Update();
}

void Block::Render() {
	VECTOR camPos = Camera::main->GetPosition();

    DrawBox(
           (int)(position.x - size.x * 0.5f - camPos.x + WINDOW_WIDTH / 2),
           (int)(position.y - size.y * 0.5f - camPos.y + WINDOW_HEIGHT / 2),

           (int)(position.x + size.x * 0.5f - camPos.x + WINDOW_WIDTH / 2),
           (int)(position.y + size.y * 0.5f - camPos.y + WINDOW_HEIGHT / 2),

           COLOR_BLUE,
           FALSE
    );

    pCollider->Render();
}