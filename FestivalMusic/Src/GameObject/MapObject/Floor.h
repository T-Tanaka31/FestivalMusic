#pragma once
#include "../GameObject.h"

class SquareCollider;

class Floor : public GameObject {
private:
    VECTOR size;

public:
    Floor(
        VECTOR _pos,
        VECTOR _size = VGet(200.0f, 200.0f, 0.0f));

    ~Floor();

public:
    void Start() override;
    void Update() override;
    void Render() override;
};