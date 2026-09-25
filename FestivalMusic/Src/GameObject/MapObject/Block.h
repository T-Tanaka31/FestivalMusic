#pragma once
#include "../GameObject.h"

class Block : public GameObject {
private:
    VECTOR size;

    int graphHandle;

public:
    Block(VECTOR pos, VECTOR size = VGet(200.0f, 200.0f, 0));

    ~Block();

public:
    void Start() override;
    void Update() override;
    void Render() override;
};

