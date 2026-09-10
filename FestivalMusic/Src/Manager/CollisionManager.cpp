#include "CollisionManager.h"
#include "../Difinition/Colors.h"

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

void CollisionManager::CheckCollision() {
	for (int i = 0; i < colliders.size(); i++) {
		for (int j = i + 1; j < colliders.size(); j++) {
			if (colliders[i]->IsHit(colliders[j])) {
				// 衝突時処理
				DrawFormatString(
					10,
					10,
					COLOR_WHITE,
					"Collision!"
				);
			}
		}
	}
}