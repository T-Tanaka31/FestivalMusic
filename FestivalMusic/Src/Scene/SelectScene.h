#pragma once

#include "Scene.h"

class SelectScene : public Scene {
public:
	SelectScene();
	~SelectScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:
	int selectedStage;
	int stageCount;
};