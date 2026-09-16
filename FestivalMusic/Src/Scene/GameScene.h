#pragma once

#include <vector>
#include "../GameObject/Character/Player/Player.h"
#include "Scene.h"
class Camera;
class Block;

class GameScene : public Scene {
private:
    Player* player;
    Camera* camera;

    std::vector<Block*> blocks;

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

    Player* GetPlayer() {
        return player;
    }

    void SetPlayerPosition(VECTOR pos) {
        player->SetPosition(pos);
    }
};