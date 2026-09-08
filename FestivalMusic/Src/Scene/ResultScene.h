#pragma once
#include "Scene.h"
class ResultScene : public Scene {
public:
	ResultScene();
	~ResultScene();

	void Init() override;
	void Update() override;
	void Draw() override;
};

