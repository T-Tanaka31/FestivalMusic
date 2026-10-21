#pragma once
#include "../GameObject.h"
class ReverseSpike : public GameObject {
private:
	VECTOR size;

	int graphHandle;

public:
	ReverseSpike(VECTOR pos, VECTOR size = VGet(200.0f, 200.0f, 0.0f));

	~ReverseSpike();

public:
	void Start() override;
	void Update() override;
	void Render() override;
};

