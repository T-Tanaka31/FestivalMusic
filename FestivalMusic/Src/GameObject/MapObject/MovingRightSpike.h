#pragma once

#include "../GameObject.h"
#include <DxLib.h>

enum class MoveRightAxis {
    Horizontal,
    Vertical
};

class MovingRightSpike : public GameObject {
private:

    VECTOR size;

    int graphHandle;

    // 移動方向
    MoveRightAxis moveAxis;

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

    MovingRightSpike(
        VECTOR _pos,
        VECTOR _size,
        MoveRightAxis _axis,
        float _distance,
        float _speed
    );

    ~MovingRightSpike();

    void Start() override;
    void Update() override;
    void Render() override;

    VECTOR GetFrameDelta() const {
        return frameDelta;
    }
};