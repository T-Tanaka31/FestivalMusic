#pragma once
#include "../GameObject.h"
class Rock : public GameObject {
private:
    VECTOR size;

    int graphHandle;

public:
    Rock(VECTOR pos, VECTOR size = VGet(200.0f, 200.0f, 0));

    ~Rock();

public:
    void Start() override;
    void Update() override;
    void Render() override;

};

