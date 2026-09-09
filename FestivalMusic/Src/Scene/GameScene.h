#pragma once
#include "Scene.h"
#include "../GameObject/Character/Player/Player.h"
class GameScene : public Scene {
private:
	Player* player;

	int graphHandle;
public:
	GameScene();
	~GameScene();

	void Init() override;
	void Update() override;
	void Draw() override;
};

