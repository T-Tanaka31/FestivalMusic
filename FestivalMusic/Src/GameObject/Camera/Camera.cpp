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
#if _DEBUG
		DrawFormatString(
			0,
			0,
			GetColor(255, 0, 0),
			"Target NULL"
		);
#endif
		return;
	}

	// プレイヤーの位置をカメラ位置にする
	position = pTarget->GetPosition();

	// =========================
	// 左端
	// =========================

	// =========================
	// マップ左右端
	// =========================

	float mapLeft = (float)MapLoader::GetMapLeft();
	float mapRight = (float)MapLoader::GetMapRight();

	// 画面の横幅の半分
	float halfScreenWidth = WINDOW_WIDTH * 0.5f;

	// マップが画面より狭い場合は中央に固定
	if (mapRight - mapLeft <= WINDOW_WIDTH) {

		position.x = (mapLeft + mapRight) * 0.5f;
	}
	else {

		float minCameraX = mapLeft + halfScreenWidth;
		float maxCameraX = mapRight - halfScreenWidth;

		if (position.x < minCameraX) {
			position.x = minCameraX;
		}

		if (position.x > maxCameraX) {
			position.x = maxCameraX;
		}
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
#if _DEBUG
	DrawFormatString(
		0,
		20,
		GetColor(255, 255, 255),
		"Cam : %.1f %.1f",
		position.x,
		position.y
	);
#endif
}

void Camera::Render() {
}