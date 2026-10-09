#pragma once
#include "../GameObject.h"
class Spike : public GameObject {
private:
	VECTOR size;

	int graphHandle;

public:
	Spike(VECTOR pos, VECTOR size = VGet(200.0f, 200.0f, 0.0f));

	~Spike();

public:
	void Start() override;
	void Update() override;
	void Render() override;
};

