#include "CollisionUtility.h"

HitDirection CollisionUtility::ResolveBoxCollision(
    SquareCollider* a,
    SquareCollider* b) {

    if (!a || !b) {
        return HitDirection::None;
    }

    // ==============================
    // 重なり量
    // ==============================

    float overlapLeft =
        a->GetMaxPoint().x - b->GetMinPoint().x;

    float overlapRight =
        b->GetMaxPoint().x - a->GetMinPoint().x;

    float overlapTop =
        a->GetMaxPoint().y - b->GetMinPoint().y;

    float overlapBottom =
        b->GetMaxPoint().y - a->GetMinPoint().y;


    // ==============================
    // 一番小さい重なりを探す
    // ==============================

    float minOverlap = overlapLeft;

    HitDirection direction = HitDirection::Left;

    if (overlapRight < minOverlap) {
        minOverlap = overlapRight;
        direction = HitDirection::Right;
    }

    if (overlapTop < minOverlap) {
        minOverlap = overlapTop;
        direction = HitDirection::Top;
    }

    if (overlapBottom < minOverlap) {
        minOverlap = overlapBottom;
        direction = HitDirection::Bottom;
    }


    // ==============================
    // GameObject取得
    // ==============================

    GameObject* objA = a->GetGameObject();
    GameObject* objB = b->GetGameObject();

    if (!objA || !objB) {
        return HitDirection::None;
    }


    // ==============================
    // Blockは動かさない
    // ==============================

    GameObject* obj = objA;

    if (objA->GetTag() == "Block") {
        obj = objB;
    }


    // ==============================
    // Enemy × Block
    // ==============================

    if (objA->GetTag() == "Enemy" &&
        objB->GetTag() == "Block") {

        obj = objA;

        switch (direction) {

        case HitDirection::Left:
            obj->SetPosition(
                VGet(
                    obj->GetPosition().x - overlapLeft,
                    obj->GetPosition().y,
                    0));
            break;

        case HitDirection::Right:
            obj->SetPosition(
                VGet(
                    obj->GetPosition().x + overlapRight,
                    obj->GetPosition().y,
                    0));
            break;

        case HitDirection::Top:
            obj->SetPosition(
                VGet(
                    obj->GetPosition().x,
                    obj->GetPosition().y - overlapTop,
                    0));
            break;

        case HitDirection::Bottom:
            obj->SetPosition(
                VGet(
                    obj->GetPosition().x,
                    obj->GetPosition().y + overlapBottom,
                    0));
            break;

        default:
            break;
        }

        a->Update();

        return direction;
    }


    // Block × Enemy
    // bがEnemyなのでbだけ動かす
    if (objA->GetTag() == "Block" &&
        objB->GetTag() == "Enemy") {

        obj = objB;

        switch (direction) {

        case HitDirection::Left:
            obj->SetPosition(
                VGet(
                    obj->GetPosition().x + overlapLeft,
                    obj->GetPosition().y,
                    0));
            break;

        case HitDirection::Right:
            obj->SetPosition(
                VGet(
                    obj->GetPosition().x - overlapRight,
                    obj->GetPosition().y,
                    0));
            break;

        case HitDirection::Top:
            obj->SetPosition(
                VGet(
                    obj->GetPosition().x,
                    obj->GetPosition().y + overlapTop,
                    0));
            break;

        case HitDirection::Bottom:
            obj->SetPosition(
                VGet(
                    obj->GetPosition().x,
                    obj->GetPosition().y - overlapBottom,
                    0));
            break;

        default:
            break;
        }

        b->Update();

        return direction;
    }


    // ==============================
    // その他の衝突
    // ==============================

    switch (direction) {

    case HitDirection::Left:
        obj->SetPosition(
            VGet(
                obj->GetPosition().x - overlapLeft,
                obj->GetPosition().y,
                0));
        break;

    case HitDirection::Right:
        obj->SetPosition(
            VGet(
                obj->GetPosition().x + overlapRight,
                obj->GetPosition().y,
                0));
        break;

    case HitDirection::Top:
        obj->SetPosition(
            VGet(
                obj->GetPosition().x,
                obj->GetPosition().y - overlapTop,
                0));
        break;

    case HitDirection::Bottom:
        obj->SetPosition(
            VGet(
                obj->GetPosition().x,
                obj->GetPosition().y + overlapBottom,
                0));
        break;

    default:
        break;
    }

    a->Update();

    return direction;
}