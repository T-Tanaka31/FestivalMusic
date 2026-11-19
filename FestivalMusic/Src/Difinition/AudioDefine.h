#pragma once
#include <string>

struct AudioData {
    std::string fullPath;
    std::string name;
    bool is3D;
};

const AudioData AudioList[] = {
    {"Res/Audio/Hit.mp3", "Hit", false},
    {"Res/Audio/Goal.mp3", "Goal", false},
    {"Res/Audio/Naruko.mp3", "Naruko", false},
    {"Res/Audio/Noise.mp3", "Noise", false},
};

const int AudioCount = sizeof(AudioList) / sizeof(AudioData);
