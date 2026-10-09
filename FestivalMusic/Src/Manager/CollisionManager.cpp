#include "../GameObject/Character/Player/Player.h"
#include "../GameObject/Character/Enemy/Enemy.h"
#include "../GameObject/MapObject/Spike.h"
#include "CollisionManager.h"
#include "../Difinition/Colors.h"
#include "../Utility/CollisionUtility.h"
#include <algorithm>

// 静的メンバ変数の初期化
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

#if _DEBUG
	DrawFormatString(
		10,
		50,
		COLOR_WHITE,
		"Collider Count : %d",
		colliders.size()
	);
#endif

	for (int i = 0; i < colliders.size(); i++) {

		for (int j = i + 1; j < colliders.size(); j++) {

			// ==========================================
			// 当たっていなければ何もしない
			// ==========================================

			if (!colliders[i]->IsHit(colliders[j])) {
				continue;
			}

			SquareCollider* a =
				dynamic_cast<SquareCollider*>(colliders[i]);

			SquareCollider* b =
				dynamic_cast<SquareCollider*>(colliders[j]);

			if (!a || !b) {
				continue;
			}

			GameObject* objA =
				a->GetGameObject();

			GameObject* objB =
				b->GetGameObject();

			if (!objA || !objB) {
				continue;
			}

			// ==========================================
			// タグ取得
			// ==========================================

			std::string tagA = objA->GetTag();
			std::string tagB = objB->GetTag();


			// ==========================================
			// Player × Spike
			// トゲに触れたら即ゲームオーバー
			// ==========================================

			if ((tagA == "Player" && tagB == "Spike") ||
				(tagA == "Spike" && tagB == "Player")) {

				Player* player = nullptr;

				if (tagA == "Player") {
					player = dynamic_cast<Player*>(objA);
				}
				else {
					player = dynamic_cast<Player*>(objB);
				}

				if (player != nullptr) {
					// 即死
					player->TakeDamage(9999);
				}

				// Spikeとの衝突では
				// 通常の物理衝突処理をしない
				continue;
			}


			// ==========================================
			// ① 物理衝突
			// ==========================================

			if (!a->IsTrigger() && !b->IsTrigger()) {

				bool enemyPlayer =
					(tagA == "Enemy" && tagB == "Player") ||
					(tagA == "Player" && tagB == "Enemy");

				bool blockBlock =
					(tagA == "Block" && tagB == "Block");

				if (!enemyPlayer && !blockBlock) {

					CollisionUtility::ResolveBoxCollision(
						a,
						b
					);
				}
			}


			// ==========================================
			// ② Enemy本体 × Player本体
			// ==========================================

			if (!a->IsTrigger() &&
				!b->IsTrigger() &&
				((tagA == "Enemy" && tagB == "Player") ||
					(tagA == "Player" && tagB == "Enemy"))) {

				Player* player = nullptr;

				if (tagA == "Player") {
					player = dynamic_cast<Player*>(objA);
				}
				else {
					player = dynamic_cast<Player*>(objB);
				}

				if (player != nullptr) {
					player->TakeDamage(1);
				}
			}


			// ==========================================
			// ③ Enemy攻撃Collider → Player
			// ==========================================

			if (a->IsTrigger() &&
				tagA == "Enemy" &&
				tagB == "Player") {

				Player* player =
					dynamic_cast<Player*>(objB);

				if (player != nullptr) {
					player->TakeDamage(1);
				}
			}


			// ==========================================
			// ④ Enemy攻撃Collider → Player
			// Colliderの順番が逆
			// ==========================================

			if (b->IsTrigger() &&
				tagB == "Enemy" &&
				tagA == "Player") {

				Player* player =
					dynamic_cast<Player*>(objA);

				if (player != nullptr) {
					player->TakeDamage(1);
				}
			}


			// ==========================================
			// ⑤ Player攻撃Collider → Enemy
			// ==========================================

			if (a->IsTrigger() &&
				tagA == "Player" &&
				tagB == "Enemy") {

				Enemy* enemy =
					dynamic_cast<Enemy*>(objB);

				if (enemy != nullptr) {
					enemy->TakeDamage(5);
				}
			}


			// ==========================================
			// ⑥ Player攻撃Collider → Enemy
			// Colliderの順番が逆
			// ==========================================

			if (b->IsTrigger() &&
				tagB == "Player" &&
				tagA == "Enemy") {

				Enemy* enemy =
					dynamic_cast<Enemy*>(objA);

				if (enemy != nullptr) {
					enemy->TakeDamage(5);
				}
			}


			// ==========================================
			// ⑦ 通常の衝突通知
			// ==========================================

			objA->OnTriggerStay(colliders[j]);
			objB->OnTriggerStay(colliders[i]);
		}
	}
}

void CollisionManager::Clear() {
	colliders.clear();
}

void CollisionManager::RemoveColliders(GameObject* obj) {
	colliders.erase(
		std::remove_if(
			colliders.begin(),
			colliders.end(),
			[obj](Collider* collider) {
				return collider->GetGameObject() == obj;
			}
		),
		colliders.end()
	);
}