#pragma once
#include "../GameObject.h"

class Camera : public GameObject {
private:
    // 追従対象
    GameObject* pTarget;

    // オフセット
    VECTOR offset;

public:
    static Camera* main;

public:
    Camera(
        VECTOR _pos = VGet(0.0f, 0.0f, 0.0f),
        VECTOR _offset = VGet(0.0f, 0.0f, 0.0f));

    ~Camera();

public:
    void Start() override;
    void Update() override;
    void Render() override;

public:
    inline GameObject* GetTarget() const {
        return pTarget;
    }

    inline void SetTarget(GameObject* _target) {
        pTarget = _target;
    }

    inline VECTOR GetOffset() const {
        return offset;
    }

    inline VECTOR GetCameraPos() const {
        return position;
    }
};