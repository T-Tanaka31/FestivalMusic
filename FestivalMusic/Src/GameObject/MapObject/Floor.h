#pragma once
#include "../GameObject.h"

class Floor : public GameObject {
private:
	VECTOR size;

	int graphHandle;

public:
	Floor(VECTOR _pos, VECTOR _size = VGet(200.0f, 200.0f, 0.0f));

	~Floor();

public:
	void Start() override;
	void Update() override;
	void Render() override;
};