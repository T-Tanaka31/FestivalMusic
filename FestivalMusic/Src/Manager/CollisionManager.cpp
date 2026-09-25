#include "CollisionManager.h"
#include "../Difinition/Colors.h"
#include "../Utility/CollisionUtility.h"

//	静的メンバ変数の初期化
CollisionManager* CollisionManager::pInstance = nullptr;

CollisionManager::CollisionManager() {
}

void CollisionManager::CreateInstance() {
	pInstance = new CollisionManager();
}

CollisionManager* CollisionManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();
	return pInstance;
}

void CollisionManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

void CollisionManager::AddCollider(Collider* col) {
    colliders.push_back(col);
}

void CollisionManager::CheckCollision() {
    DrawFormatString(
        10,
        50,
        COLOR_WHITE,
        "Collider Count : %d",
        colliders.size()
    );

    for (int i = 0; i < colliders.size(); i++) {
        for (int j = i + 1; j < colliders.size(); j++) {
			if (colliders[i]->IsHit(colliders[j])) {

				SquareCollider* a =
					dynamic_cast<SquareCollider*>(colliders[i]);

				SquareCollider* b =
					dynamic_cast<SquareCollider*>(colliders[j]);

				if (!a || !b) {
					continue;
				}

				// ==============================
				// 物理衝突
				// ==============================

				if (!a->IsTrigger() && !b->IsTrigger()) {
					CollisionUtility::ResolveBoxCollision(a, b);
				}

				// ==============================
				// 衝突通知
				// ==============================

				a->GetGameObject()->OnTriggerStay(colliders[j]);

				b->GetGameObject()->OnTriggerStay(colliders[i]);
			}
        }
    }
}



void CollisionManager::Clear() {
    colliders.clear();
}
