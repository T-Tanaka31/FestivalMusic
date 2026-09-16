#pragma once
#include "../GameObject.h"

class SquareCollider;

class Wall : public GameObject {
private:
	VECTOR size;

public:
	Wall(VECTOR _pos, VECTOR _size = VGet(200.0f, 200.0f, 0.0f));

	~Wall();

public:
	void Start() override;
	void Update() override;
	void Render() override;
};

