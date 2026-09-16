#include "Camera.h"

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
        DrawFormatString(0, 0, GetColor(255, 0, 0),
            "Target NULL");
        return;
    }

    position = pTarget->GetPosition();

    DrawFormatString(
        0, 20,
        GetColor(255, 255, 255),
        "Cam : %.1f %.1f",
        position.x,
        position.y);
}

void Camera::Render() {
}