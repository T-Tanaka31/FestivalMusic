#pragma once
#include "../GameObject.h"

class SquareCollider;

class Block : public GameObject {
private:
    VECTOR size;

public:
    Block(VECTOR pos, VECTOR size = VGet(200.0f, 200.0f, 0));

    ~Block();

public:
    void Start() override;
    void Update() override;
    void Render() override;
};

