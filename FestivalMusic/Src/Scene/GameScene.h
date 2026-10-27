#pragma once

#include <vector>
#include "../GameObject/Character/Player/Player.h"
#include "../GameObject/Character/Enemy/Enemy.h"
#include "../GameObject/Character/Enemy/Boss/Boss.h"
#include "Scene.h"
class Camera;
class Block;
class Floor;
class Question;
class Brick;
class Rock;
class Goal;
class Spike;
class ReverseSpike;

class GameScene : public Scene {
private:
    Player* player;
    Camera* camera;
    Goal* goal;

    std::vector<Block*> blocks;
    std::vector<Floor*> floors;
    std::vector<Brick*> bricks;
    std::vector<Question*> queses;
    std::vector<Rock*> rocks;
	std::vector<Spike*> spikes;
	std::vector<ReverseSpike*> reverseSpikes;
    std::vector<Enemy*> enemies;
	std::vector<Boss*> bosses;

    int graphHandle;

    int stage;

    // ステージ開始演出
    bool isStageStarting;
    int stageStartTimer;

    // ステージクリア演出
    bool isStageClearing;
    int stageClearTimer;
public:
    GameScene(int _stage);
    ~GameScene();

public:
    void Init();
    void Update();
    void Draw();

public:
    void AddBlock(Block* block);
    void AddFloor(Floor* floor);
    void AddBrick(Brick* brick);
    void AddQues(Question* ques);
    void AddRock(Rock* rock);
	void AddSpike(Spike* spike);
	void AddReverseSpike(ReverseSpike* reverseSpike);
    
    void AddEnemy(Enemy* enemy);

    void AddBoss(Boss* boss);

    void AddFlyingEnemy(Enemy* enemy);

    void SetGoal(Goal* pGoal) { goal = pGoal; }

    Player* GetPlayer() {
        return player;
    }

    void SetPlayerPosition(VECTOR pos) {
        player->SetPosition(pos);
    }
};