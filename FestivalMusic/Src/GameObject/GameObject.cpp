#include "GameObject.h"


GameObject::GameObject(VECTOR _pos, std::string _tag)
	: isVisible(true)
	, position(_pos)
	, scale(VOne)
	, rotation(VZero)
	, matrix(MGetIdent())
	, tag(_tag)
	, isAlive(TRUE)
	, GraphHandle(0)
	, velocity(VZero) {
}

GameObject::~GameObject() {
}

void GameObject::Update() {
	//	非表示なら更新しない
	if (!isVisible)
		return;

	//	座標、回転、拡縮から行列を求める


	MATRIX mRotX = MGetRotX(Deg2Rad(rotation.x));		//	X軸回転行列
	MATRIX mRotY = MGetRotY(Deg2Rad(rotation.y));		//	Y軸回転行列
	MATRIX mRotZ = MGetRotZ(Deg2Rad(rotation.z));		//	Z軸回転行列

	//	X->Y->Z の順で回転行列を作成する
	MATRIX mRotXYZ = MMult(MMult(mRotX, mRotY), mRotZ);

	//	拡縮行列を取得する
	MATRIX mScale = MGetScale(scale);

	//	平行移動行列を取得する
	MATRIX mTranslate = MGetTranslate(position);

	//	行列の乗算は合成
	//	回転行列 -> 拡縮行列 -> 平行行列 の順に掛け合わせる
	//	(交換法則は成り立たない)
	matrix = MMult(MMult(mRotXYZ, mScale), mTranslate);
}

void GameObject::OnTriggerEnter(Collider* _pOther) {
}

void GameObject::OnTriggerStay(Collider* _pOther) {
}

void GameObject::OnTriggerExit(Collider* _pOther) {
}
