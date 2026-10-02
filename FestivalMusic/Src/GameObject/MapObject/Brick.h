#pragma once
#include "../GameObject.h"
class Brick : public GameObject {
private:
    VECTOR size;

    int graphHandle;

public:
    Brick(VECTOR pos, VECTOR size = VGet(200.0f, 200.0f, 0));

    ~Brick();

public:
    void Start() override;
    void Update() override;
    void Render() override;

};

