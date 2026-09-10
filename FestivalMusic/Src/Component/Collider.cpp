#include "Collider.h"
#include "../Difinition/Colors.h"

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

SquareCollider::SquareCollider(
	GameObject* obj,
	VECTOR _size
)
	: Collider(obj)
	, size(_size) {
}
void SquareCollider::Update() {
	VECTOR pos = pGameObject->GetPosition();

	minPoint.x = pos.x - 25;
	minPoint.y = pos.y - 25;

	maxPoint.x = pos.x + 25;
	maxPoint.y = pos.y + 25;
}

void SquareCollider::Render() {
	DrawBox(
		(int)minPoint.x,
		(int)minPoint.y,
		(int)maxPoint.x,
		(int)maxPoint.y,
		COLOR_RED,
		FALSE
	);
}
