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
			// Playerの攻撃判定を通常のPlayer判定と区別
			// ==========================================

			// Playerの攻撃ColliderはTriggerなので、
			// Trigger × Block / Spike の物理処理を行わない

			if ((a->IsTrigger() && tagA == "Player") ||
				(b->IsTrigger() && tagB == "Player")) {

				bool hitBlock =
					(tagA == "Block" || tagB == "Block");

				bool hitSpike =
					(tagA == "Spike" || tagB == "Spike");

				bool hitGoal =
					(tagA == "Goal" || tagB == "Goal");

				if (hitBlock || hitSpike || hitGoal) {
					continue;
				}
			}

			// ==========================================
			// Player本体 × Spike
			// トゲに触れたら即ゲームオーバー
			// ==========================================

			if (!a->IsTrigger() &&
				!b->IsTrigger() &&
				((tagA == "Player" && tagB == "Spike") ||
					(tagA == "Spike" && tagB == "Player"))) {

				Player* player = nullptr;

				if (tagA == "Player") {
					player = dynamic_cast<Player*>(objA);
				}
				else {
					player = dynamic_cast<Player*>(objB);
				}

				if (player != nullptr) {
					player->DieImmediately();
				}

				// Spikeとの通常の物理衝突処理はしない
				continue;
			}

			// ==========================================
			// 敵同士の物理衝突
			// 両方の敵を押し戻して、重ならないようにする
			// ==========================================
			if (!a->IsTrigger() &&
				!b->IsTrigger() &&
				tagA == "Enemy" &&
				tagB == "Enemy") {

				CollisionUtility::ResolveEnemyCollision(a, b);
			}

			// ==========================================
			// ① 通常の物理衝突
			// Trigger判定は物理衝突させない
			// ==========================================
			if (!a->IsTrigger() && !b->IsTrigger()) {

				bool enemyEnemy =
					(tagA == "Enemy" && tagB == "Enemy");

				bool enemyPlayer =
					(tagA == "Enemy" && tagB == "Player") ||
					(tagA == "Player" && tagB == "Enemy");

				bool blockBlock =
					(tagA == "Block" && tagB == "Block");

				if (!enemyPlayer && !enemyEnemy && !blockBlock) {
					CollisionUtility::ResolveBoxCollision(a, b);
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