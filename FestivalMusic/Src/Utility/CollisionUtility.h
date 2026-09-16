#pragma once
#include "../Component/Collider.h"
#include "../Enum/HitDirection.h"

class CollisionUtility {
public:
    static HitDirection ResolveBoxCollision(
        SquareCollider* a,
        SquareCollider* b);
};
