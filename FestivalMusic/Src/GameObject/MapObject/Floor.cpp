#include "Floor.h"
#include "../Camera/Camera.h"
#include "../../Component/Collider.h"
#include "../../Difinition/Constant.h"
#include "../../Difinition/Colors.h"

Floor::Floor(VECTOR _pos, VECTOR _size)
    : GameObject(_pos, "Floor")
    , size(_size) {
    pCollider = new SquareCollider(this, size);
}

Floor::~Floor() {
}

void Floor::Start() {
}

void Floor::Update() {
    pCollider->Update();
}

void Floor::Render() {
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