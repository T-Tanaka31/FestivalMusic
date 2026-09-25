#pragma once

#include <vector>
#include "../GameObject/Character/Player/Player.h"
#include "../GameObject/Character/Enemy/Enemy.h"
#include "Scene.h"
class Camera;
class Block;
class Goal;

class GameScene : public Scene {
private:
    Player* player;
    Camera* camera;
    Goal* goal;

    std::vector<Block*> blocks;
    std::vector<Enemy*> enemies;

    int graphHandle;

public:
    GameScene();
    ~GameScene();

public:
    void Init();
    void Update();
    void Draw();

public:
    void AddBlock(Block* block);
    
    void AddEnemy(Enemy* enemy);

    void SetGoal(Goal* pGoal) { goal = pGoal; }

    Player* GetPlayer() {
        return player;
    }

    void SetPlayerPosition(VECTOR pos) {
        player->SetPosition(pos);
    }
};