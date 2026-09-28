#include "Camera.h"
#include "../../Difinition/Constant.h"
#include "../../Map/MapLoader.h"

Camera* Camera::main = nullptr;

Camera::Camera(VECTOR _pos, VECTOR _offset)
    : GameObject(_pos, "Camera")
    , pTarget(nullptr)
    , offset(_offset) {
    main = this;
}

Camera::~Camera() {
}

void Camera::Start() {
}

void Camera::Update() {

    if (pTarget == nullptr) {
        DrawFormatString(
            0,
            0,
            GetColor(255, 0, 0),
            "Target NULL"
        );
        return;
    }

    // プレイヤーの位置をカメラ位置にする
    position = pTarget->GetPosition();

    // =========================
    // 左端
    // =========================

    float minCameraX =
        CAMERA_WIDTH * 0.5f;

    if (position.x < minCameraX) {
        position.x = minCameraX;
    }

    // =========================
    // マップ下端
    // =========================

    float mapBottom =
        (float)MapLoader::GetMapBottom();

    float maxCameraY =
        mapBottom - CAMERA_HEIGHT * 0.5f;

    if (position.y > maxCameraY) {
        position.y = maxCameraY;
    }

    DrawFormatString(
        0,
        20,
        GetColor(255, 255, 255),
        "Cam : %.1f %.1f",
        position.x,
        position.y
    );
}

void Camera::Render() {
}