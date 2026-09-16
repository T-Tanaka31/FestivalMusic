#include "Collider.h"
#include "../Difinition/Colors.h"
#include "../Difinition/Constant.h"
#include "../GameObject/Camera/Camera.h"
#include "../Manager/CollisionManager.h"

Collider::Collider(GameObject* _pObj)
	: isEnable(TRUE)
	, pGameObject(_pObj)
	, offset(VZero) {
}

CircleCollider::CircleCollider(GameObject* _pObj, float _r)
	: Collider(_pObj)
	, radius(_r)
	, center(0) {
}

void CircleCollider::Update() {
	if (!isEnable) return;

	VECTOR pos = pGameObject->GetPosition();

	center.x = pos.x + offset.x;
	center.y = pos.y + offset.y;
	center.z = 0;
}

void CircleCollider::Render() {
#if _DEBUG
	DrawCircle(
		(int)center.x,
		(int)center.y,
		(int)radius,
		COLOR_RED,
		FALSE
	);
#endif
}

bool CircleCollider::IsHit(Collider* other) {
	return false;
}

SquareCollider::SquareCollider(
	GameObject* obj,
	VECTOR _size
)
	: Collider(obj)
	, size(_size) {
	CollisionManager::GetInstance()->AddCollider(this);
}

void SquareCollider::Update() {
	VECTOR pos = pGameObject->GetPosition();

	minPoint.x = pos.x - size.x * 0.5f;
	minPoint.y = pos.y - size.y * 0.5f;

	maxPoint.x = pos.x + size.x * 0.5f;
	maxPoint.y = pos.y + size.y * 0.5f;
}

void SquareCollider::Render() {
	VECTOR camPos = Camera::main->GetPosition();

	DrawBox(
		(int)(minPoint.x - camPos.x + WINDOW_WIDTH / 2),
		(int)(minPoint.y - camPos.y + WINDOW_HEIGHT / 2),
		(int)(maxPoint.x - camPos.x + WINDOW_WIDTH / 2),
		(int)(maxPoint.y - camPos.y + WINDOW_HEIGHT / 2),
		COLOR_RED,
		FALSE
	);
}

bool SquareCollider::IsHit(Collider* other) {
	SquareCollider* sq = dynamic_cast<SquareCollider*>(other);

	if (!sq) return false;

	return
		minPoint.x < sq->maxPoint.x &&
		maxPoint.x > sq->minPoint.x &&
		minPoint.y < sq->maxPoint.y &&
		maxPoint.y > sq->minPoint.y;
}
