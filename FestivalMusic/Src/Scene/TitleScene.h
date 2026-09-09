#pragma once
#include "Scene.h"
class TitleScene : public Scene {
	int graphHandle;
public:
	TitleScene();
	~TitleScene();

	void Init() override;
	void Update() override;
	void Draw() override;
};

