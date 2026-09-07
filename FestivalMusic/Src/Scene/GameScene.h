#pragma once
#include "Scene.h"
class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void Init() override;
	void Update() override;
	void Draw() override;
};

