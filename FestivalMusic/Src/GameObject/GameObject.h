#pragma once
#include "../Difinition/VectorDifines.h"
#include "../Difinition/Macros.h"
#include <string>
class GameObject {
protected:
	bool isVisible;		//	表示フラグ
	VECTOR position;	//	座標
	VECTOR rotation;	//	回転角
	VECTOR scale;		//	拡縮率

	MATRIX matrix;		//	変換行列

	std::string tag;	//	タグ

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
	inline void SetPosition(VECTOR _pos) { position = _pos; }

	/*
	 * @brief	座標を設定する
	 */
	inline void SetPosition(float _x, float _y, float _z) { position = VGet(_x, _y, _z); }

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
};

