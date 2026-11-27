#include "MovingRightSpike.h"
#include "../Camera/Camera.h"
#include "../../Component/Collider.h"
#include "../../Difinition/Constant.h"
#include "../../Difinition/Colors.h"

MovingRightSpike::MovingRightSpike(
    VECTOR _pos,
    VECTOR _size,
    MoveRightAxis _axis,
    float _distance,
    float _speed
)
    : GameObject(_pos, "Spike")
    , size(_size)
    , graphHandle(0)
    , moveAxis(_axis)
    , startPosition(_pos)
    , moveDistance(_distance)
    , moveSpeed(_speed)
    , moveDirection(1)
    , frameDelta(VZero) {
    pCollider = new SquareCollider(this, VGet(size.x * 0.85f, size.y * 0.85f, size.z));
    Start();
}

MovingRightSpike::~MovingRightSpike() {
}

void MovingRightSpike::Start() {
    graphHandle = LoadGraph("Res/Block/rightSpike.png");
}

void MovingRightSpike::Update() {

    VECTOR oldPosition = position;

    if (moveAxis == MoveRightAxis::Horizontal) {

        position.x += moveSpeed * moveDirection;

        if (position.x >= startPosition.x + moveDistance) {
            position.x = startPosition.x + moveDistance;
            moveDirection = -1;
        }
        else if (position.x <= startPosition.x - moveDistance) {
            position.x = startPosition.x - moveDistance;
            moveDirection = 1;
        }
    }
    else if (moveAxis == MoveRightAxis::Vertical) {

        position.y += moveSpeed * moveDirection;

        if (position.y >= startPosition.y + moveDistance) {
            position.y = startPosition.y + moveDistance;
            moveDirection = -1;
        }
        else if (position.y <= startPosition.y - moveDistance) {
            position.y = startPosition.y - moveDistance;
            moveDirection = 1;
        }
    }

    // 1フレームの移動量
    frameDelta = VSub(position, oldPosition);

    pCollider->Update();
}

void MovingRightSpike::Render() {

    VECTOR camPos = Camera::main->GetPosition();

    int drawX =
        (int)(position.x - size.x * 0.5f
            - camPos.x + WINDOW_WIDTH / 2);

    int drawY =
        (int)(position.y - size.y * 0.5f
            - camPos.y + WINDOW_HEIGHT / 2);

    const int TILE_SIZE = 64;

    int tileCountX = (int)(size.x / TILE_SIZE);
    int tileCountY = (int)(size.y / TILE_SIZE);

    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            DrawExtendGraph(
                drawX + x * TILE_SIZE,
                drawY + y * TILE_SIZE,
                drawX + (x + 1) * TILE_SIZE,
                drawY + (y + 1) * TILE_SIZE,
                graphHandle,
                TRUE
            );
        }
    }
}