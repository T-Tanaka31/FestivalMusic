#pragma once

#include <vector>
#include <string>

class GameScene;

class MapLoader {
public:
    static bool Load(
        const std::string& path,
        GameScene* scene);
};