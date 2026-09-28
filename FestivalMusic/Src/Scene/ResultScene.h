#pragma once

#include "Scene.h"

class ResultScene : public Scene {
public:

	enum class ResultType {
		GameOver,
		GameClear
	};

	ResultScene();
	~ResultScene();

	void Init() override;
	void Update() override;
	void Draw() override;

	void SetResult(ResultType type);

private:

	ResultType resultType;
};