#pragma once
#include "../GameObject.h"
class Question :public GameObject {
private:
    VECTOR size;

    int graphHandle;

public:
    Question(VECTOR pos, VECTOR size = VGet(200.0f, 200.0f, 0));

    ~Question();

public:
    void Start() override;
    void Update() override;
    void Render() override;
};

