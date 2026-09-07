#pragma once
class SceneManager {
#pragma region シングルトンのデータ構造
private:
	static SceneManager* pInstance;

private:
	/*
	 *	@brief	コンストラクタ
	 *	@tips	外部で生成されないようにアクセス指定子をprivateにする
	 */
	SceneManager();

	/*
	 *	@brief	デストラクタ
	 */
	~SceneManager() = default;

public:
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator = (const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator = (SceneManager&&) = delete;

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
	 *	@return		SceneManager*	自身のインスタンスのアドレス
	 */
	static SceneManager* GetInstance();

	/*
	 *	@function	DestroyInstance
	 *	@brief		自身のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion
};

