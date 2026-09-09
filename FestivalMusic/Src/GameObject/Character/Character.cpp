#include "Character.h"

Character::Character(VECTOR _pos, std::string _tag)
	: GameObject(_pos, _tag)
	, direction(Direction::Down)
	, hp(100)
	, maxHp(100)
	, atk(10)
	, def(5)
	, moveSpeed(5.0f) {
}	

Character::~Character() {
}

void Character::Damage(Character* _attacker, int _rawDamage) {
	if (IsDead()) return;

	//	ダメージ計算
	int damage = _rawDamage - def;
	if (damage < 0) damage = 0;
	//	体力を減らす
	hp -= damage;
	//	体力が0以下になったら死亡処理
	if (hp <= 0) {
		isAlive = false;
 	}
}