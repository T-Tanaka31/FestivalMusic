#pragma once
#include "../Difinition/VectorDifines.h"
#include "../Difinition/Macros.h"
#include <string>
class GameObject {
protected:
	bool isVisible;		//	表示フラグ
	bool isAlive;		//	生存フラグ

	VECTOR position;	//	座標
	VECTOR rotation;	//	回転角
	VECTOR scale;		//	拡縮率

	MATRIX matrix;		//	変換行列

	std::string tag;	//	タグ

	int GraphHandle;	//	グラフィックハンドル

	VECTOR velocity;	//	速度

public:
	/*
	 *	@brief	コンストラクタ
	 *	@param	_pos	座標
	 *	@param	_tag	タグ
	 */
	GameObject(VECTOR _pos = VZero, std::string _tag = "");

	/*
	 *	@brief	デストラクタ
	 */
	virtual ~GameObject();

public:
	/*
	 *	@function	Start
	 *	@brief		初期化処理
	 */
	virtual void Start() = 0;

	/*
	 *	@function	Update
	 *	@brief		更新処理
	 */
	virtual void Update() = 0;

	/*
	 *	@function	Render
	 *	@brief		描画処理
	 */
	virtual void Render() = 0;

public:	//	Getterm,Setter
	/*
	 *	@brief	表示フラグを取得する
	 */
	inline bool IsVisible() const { return isVisible; }

	/*
	 *	@brief	表示フラグを設定する
	 */
	inline void SetVisible(bool _v) { isVisible = _v; }

	/*
	 *	@brief	座標を取得する
	 */
	inline VECTOR GetPosition() const { return position; }

	/*
	 * @brief	座標を設定する
	 */
	inline virtual void SetPosition(VECTOR _pos) { position = _pos; }

	/*
	 * @brief	座標を設定する
	 */
	inline virtual void SetPosition(float _x, float _y, float _z) { position = VGet(_x, _y, _z); }

	/*
	 *	@brief	回転角を取得する
	 */
	inline VECTOR GetRotation() const { return rotation; }

	/*
	 * @brief	回転角を設定する
	 */
	inline void SetRotation(VECTOR _rot) { rotation = _rot; }

	/*
	 *	@brief	変換行列を取得する
	 */ 
	inline MATRIX GetMatrix() const { return matrix; }

	/*
	 *	@brief	拡縮率を取得する
	 */
	inline VECTOR GetScale() const { return scale; }

	/*
	 *	@brief	拡縮率を設定する
	 */
	inline void SetScale(VECTOR _scale) { scale = _scale; }

	/*
	 *	@brief	拡縮率を設定する
	 */
	inline void SetScale(float _x, float _y, float _z) { scale = VGet(_x, _y, _z); }

	/*
	 *	@brief	タグを取得する
	 */
	inline std::string GetTag() const { return tag; }

	/*
	 *	@brief	タグを設定する
	 */
	inline void SetTag(std::string _tag) { tag = _tag; }

	/*
	 *	@brief	速度を取得する
	 */
	inline VECTOR GetVelocity() const { return velocity; }

	/*
	 *	@brief	速度を設定する
	 */
	inline void SetVelocity(VECTOR _vel) { velocity = _vel; }

	/*
	 *	@brief	速度を設定する
	 */
	inline void SetVelocity(float _x, float _y, float _z) { velocity = VGet(_x, _y, _z); }

	/*
	 *	@brief	速度を加算する
	 */
	inline void AddVelocity(VECTOR _vel) { velocity = VAdd(velocity, _vel); }

	/*
	 *	@brief	速度を加算する
	 */
	inline void AddVelocity(float _x, float _y, float _z) { velocity = VAdd(velocity, VGet(_x, _y, _z)); }

	/*
	 *	@brief	速度のX成分を加算する
	 */
	inline void AddVelocityX(float _x) { velocity.x += _x; }

	/*
	 *	@brief	速度のY成分を加算する
	 */
	inline void AddVelocityY(float _y) { velocity.y += _y; }

	/*
	 *	@brief	速度のZ成分を加算する
	 */
	inline void AddVelocityZ(float _z) { velocity.z += _z; }

	/*
	 *	@brief	グラフィックハンドルを取得する
	 */
	inline int GetGraphHandle() const { return GraphHandle; }

	/*
	 *	@brief	グラフィックハンドルを設定する
	 */
	inline void SetGraphHandle(int _handle) { GraphHandle = _handle; }

	/*
	 *	@brief	生存フラグを取得する
	 */
	inline bool IsAlive() const { return isAlive; }

	/*
	 *	@brief	生存フラグを設定する
	 */
	inline void SetAlive(bool _alive) { isAlive = _alive; }

};

