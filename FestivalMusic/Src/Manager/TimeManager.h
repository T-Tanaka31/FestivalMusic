#pragma once
class TimeManager {
#pragma region singleton
private:
	static TimeManager* pInstance;

private:
	/*
	 *	@brief	コンストラクタ
	 *	@tips	外部で生成されないようにアクセス指定子をprivateにする
	 */
	TimeManager();

	/*
	 *	@brief	デストラクタ
	 */
	~TimeManager() = default;

public:
	TimeManager(const TimeManager&) = delete;
	TimeManager& operator = (const TimeManager&) = delete;
	TimeManager(TimeManager&&) = delete;
	TimeManager& operator = (TimeManager&&) = delete;

private:	//	静的メンバ関数
	/*
	 *	@function	CreateInstance
	 *	@brief		自身のインスタンスを生成する
	 */
	static void CreateInstance();

public:		//	静的メンバ関数
	/*
	 *	@function	GetInstane
	 *	@brief		自身のインスタンスを取得する唯一の手段
	 *	@return		InputMangaer*	自身のインスタンスのアドレス
	 */
	static TimeManager* GetInstance();

	/*
	 *	@function	DestroyInstance
	 *	@brief		自身のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();

private:
	int prev;
	int current;
	int m = 0, s = 0, ms = 0;

public:
	float deltaTime;

public:
	void Start();
	void Update();
	void Render();

	inline int GetCurrent() const { return current; }
};

