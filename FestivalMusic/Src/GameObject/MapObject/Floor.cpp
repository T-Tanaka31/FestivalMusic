#include "Floor.h"
#include "../Camera/Camera.h"
#include "../../Component/Collider.h"
#include "../../Difinition/Constant.h"
#include "../../Difinition/Colors.h"

Floor::Floor(VECTOR _pos, VECTOR _size)
	:GameObject(_pos, "Block")
	, size(_size)
	, graphHandle(0) {
	pCollider = new SquareCollider(this, size);
	Start();
}

Floor::~Floor() {
}

void Floor::Start() {
	graphHandle = LoadGraph("Res/Block/spike.png");
}

void Floor::Update() {
	pCollider->Update();
}

void Floor::Render() {
    VECTOR camPos = Camera::main->GetPosition();

    int drawX =
        (int)(position.x - size.x * 0.5f
            - camPos.x + WINDOW_WIDTH / 2);

    int drawY =
        (int)(position.y - size.y * 0.5f
            - camPos.y + WINDOW_HEIGHT / 2);

    DrawExtendGraph(
        drawX,
        drawY,
        drawX + (int)size.x,
        drawY + (int)size.y,
        graphHandle,
        TRUE);
}