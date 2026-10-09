#include "Rock.h"
#include "../Camera/Camera.h"
#include "../../Component/Collider.h"
#include "../../Difinition/Constant.h"
#include "../../Difinition/Colors.h"

Rock::Rock(VECTOR _pos, VECTOR _size)
    : GameObject(_pos, "Block")
    , size(_size)
    , graphHandle(0) {
    pCollider = new SquareCollider(this, size);
    Start();
}

Rock::~Rock() {
}

void Rock::Start() {
    graphHandle = LoadGraph("Res/Block/tall_grass_block.png");
}

void Rock::Update() {
    pCollider->Update();
}

void Rock::Render() {
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