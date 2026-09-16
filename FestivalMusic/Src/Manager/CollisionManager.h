#pragma once
#include "../Component/Collider.h"
#include <vector>

class CollisionManager {
private:
	static CollisionManager* pInstance;

private:
	/*
	 *	@brief	コンストラクタ
	 *	@tips	外部で生成されないようにアクセス指定子をprivateにする
	 */
	CollisionManager();

	/*
	 *	@brief	デストラクタ
	 */
	~CollisionManager() = default;

public:
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator = (const CollisionManager&) = delete;
	CollisionManager(CollisionManager&&) = delete;
	CollisionManager& operator = (CollisionManager&&) = delete;

private:	//	静的メンバ関数
	/*
	 *	@function	CreateInstance
	 *	@brief		自身のインスタンスを生成する
	 */
	static void CreateInstance();

public:		//	静的メンバ関数
	/*
	 *	@function	GetInstance
	 *	@brief		自身のインスタンスを取得する唯一の手段
	 *	@return		CollisionManager*	自身のインスタンスのアドレス
	 */
	static CollisionManager* GetInstance();

	/*
	 *	@function	DestroyInstance
	 *	@brief		自身のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();

private:
	std::vector<Collider*> colliders;

public:
	void AddCollider(Collider* col);
	void RemoveCollider(Collider* col);

	void CheckCollision();

	void Clear();
};

