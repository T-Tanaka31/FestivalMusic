#pragma once
#include "../Component/Collider.h"
#include "../Enum/HitDirection.h"

class CollisionUtility {
public:
    static HitDirection ResolveBoxCollision(
        SquareCollider* a,
        SquareCollider* b);

    // 敵同士の衝突：両方を押し戻す
    static HitDirection ResolveEnemyCollision(
        SquareCollider* a,
        SquareCollider* b);
};
