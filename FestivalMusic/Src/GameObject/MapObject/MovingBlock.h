#pragma once

#include "../GameObject.h"
#include <DxLib.h>

enum class MoveAxis {
    Horizontal,
    Vertical
};

class MovingBlock : public GameObject {
private:

    VECTOR size;

    int graphHandle;

    // 移動方向
    MoveAxis moveAxis;

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

    MovingBlock(
        VECTOR _pos,
        VECTOR _size,
        MoveAxis _axis,
        float _distance,
        float _speed
    );

    ~MovingBlock();

    void Start() override;
    void Update() override;
    void Render() override;

    VECTOR GetFrameDelta() const {
        return frameDelta;
    }
};