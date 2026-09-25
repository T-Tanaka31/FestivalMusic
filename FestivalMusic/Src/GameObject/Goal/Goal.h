#pragma once
#include "../GameObject.h"
class Goal : public GameObject {
private:
	VECTOR size;

	int graphHandle;

public:
	Goal(VECTOR pos, VECTOR size = VGet(200.0f, 200.0f, 0.0f));

	~Goal();

public:
	void Start() override;
	void Update() override;
	void Render() override;
};

