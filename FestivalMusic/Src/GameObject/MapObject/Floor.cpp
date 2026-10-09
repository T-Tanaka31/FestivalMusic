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
    graphHandle = LoadGraph("Res/Block/stone_emblem.png");
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