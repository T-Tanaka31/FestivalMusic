#pragma once

#include "../GameObject.h"
#include <DxLib.h>

enum class MoveLeftAxis {
    Horizontal,
    Vertical
};

class MovingLeftSpike : public GameObject {
private:

    VECTOR size;

    int graphHandle;

    // 移動方向
    MoveLeftAxis moveAxis;

    // 初期位置
    VECTOR startPosition;

    // 移動距離
    float moveDistance;

    // 移動速度
    float moveSpeed;

    // 移動方向
    int moveDirection;

    VECTOR frameDelta;

public:

    MovingLeftSpike(
        VECTOR _pos,
        VECTOR _size,
        MoveLeftAxis _axis,
        float _distance,
        float _speed
    );

    ~MovingLeftSpike();

    void Start() override;
    void Update() override;
    void Render() override;

    VECTOR GetFrameDelta() const {
        return frameDelta;
    }
};