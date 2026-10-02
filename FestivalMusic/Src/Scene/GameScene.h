#pragma once

#include <vector>
#include "../GameObject/Character/Player/Player.h"
#include "../GameObject/Character/Enemy/Enemy.h"
#include "Scene.h"
class Camera;
class Block;
class Floor;
class Question;
class Brick;
class Rock;
class Goal;

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
    std::vector<Enemy*> enemies;

    int graphHandle;

    int stage;

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
    
    void AddEnemy(Enemy* enemy);

    void SetGoal(Goal* pGoal) { goal = pGoal; }

    Player* GetPlayer() {
        return player;
    }

    void SetPlayerPosition(VECTOR pos) {
        player->SetPosition(pos);
    }
};