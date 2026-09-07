#pragma once
#include "Scene.h"
class TitleScene : public Scene {
public:
	TitleScene();
	~TitleScene();

	void Init() override;
	void Update() override;
	void Draw() override;
};

