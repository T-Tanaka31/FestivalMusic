#include "Spike.h"
#include "../Camera/Camera.h"
#include "../../Component/Collider.h"
#include "../../Difinition/Constant.h"
#include "../../Difinition/Colors.h"

Spike::Spike(VECTOR _pos, VECTOR _size)
	: GameObject(_pos, "Spike")
	, size(_size)
	, graphHandle(0) {
	pCollider = new SquareCollider(this, size);
	Start();
}

Spike::~Spike() {
}

void Spike::Start() {
	graphHandle = LoadGraph("Res/Block/spikes.png");
}

void Spike::Update() {
	pCollider->Update();
}

void Spike::Render() {
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