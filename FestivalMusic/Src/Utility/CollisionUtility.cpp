#include "CollisionUtility.h"
#include <algorithm>

HitDirection CollisionUtility::ResolveBoxCollision(
    SquareCollider* a,
    SquareCollider* b) {
    if (!a || !b)
        return HitDirection::None;

    GameObject* objA = a->GetGameObject();
    GameObject* objB = b->GetGameObject();

    if (!objA || !objB)
        return HitDirection::None;

    SquareCollider* movingCollider = nullptr;
    SquareCollider* blockCollider = nullptr;

    if (objA->GetTag() == "Block") {
        blockCollider = a;
        movingCollider = b;
    }
    else if (objB->GetTag() == "Block") {
        blockCollider = b;
        movingCollider = a;
    }
    else {
        return HitDirection::None;
    }

    VECTOR movingPos = movingCollider->GetGameObject()->GetPosition();
    VECTOR blockPos = blockCollider->GetGameObject()->GetPosition();

    float overlapLeft =
        movingCollider->GetMaxPoint().x -
        blockCollider->GetMinPoint().x;

    float overlapRight =
        blockCollider->GetMaxPoint().x -
        movingCollider->GetMinPoint().x;

    float overlapTop =
        movingCollider->GetMaxPoint().y -
        blockCollider->GetMinPoint().y;

    float overlapBottom =
        blockCollider->GetMaxPoint().y -
        movingCollider->GetMinPoint().y;

    // 本当に重なっているか確認
    if (overlapLeft <= 0.0f ||
        overlapRight <= 0.0f ||
        overlapTop <= 0.0f ||
        overlapBottom <= 0.0f) {
        return HitDirection::None;
    }

    // 横方向のめり込みが小さいなら横方向を解決
    if (overlapLeft < overlapRight &&
        overlapLeft < overlapTop &&
        overlapLeft < overlapBottom) {
        movingPos.x -= overlapLeft;

        movingCollider->GetGameObject()->SetPosition(movingPos);
        movingCollider->Update();

        return HitDirection::Left;
    }

    if (overlapRight < overlapTop &&
        overlapRight < overlapBottom) {
        movingPos.x += overlapRight;

        movingCollider->GetGameObject()->SetPosition(movingPos);
        movingCollider->Update();

        return HitDirection::Right;
    }

    // 縦方向
    if (overlapTop < overlapBottom) {
        movingPos.y -= overlapTop;

        movingCollider->GetGameObject()->SetPosition(movingPos);
        movingCollider->Update();

        return HitDirection::Top;
    }

    movingPos.y += overlapBottom;

    movingCollider->GetGameObject()->SetPosition(movingPos);
    movingCollider->Update();

    return HitDirection::Bottom;
}