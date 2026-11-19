#pragma once

#include "../GameObject.h"
#include <DxLib.h>

enum class MoveDefaultAxis {
    Horizontal,
    Vertical
};

class MovingSpike : public GameObject {
private:

    VECTOR size;

    int graphHandle;

    // 移動方向
    MoveDefaultAxis moveAxis;

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

    MovingSpike(
        VECTOR _pos,
        VECTOR _size,
        MoveDefaultAxis _axis,
        float _distance,
        float _speed
    );

    ~MovingSpike();

    void Start() override;
    void Update() override;
    void Render() override;

    VECTOR GetFrameDelta() const {
        return frameDelta;
    }
};