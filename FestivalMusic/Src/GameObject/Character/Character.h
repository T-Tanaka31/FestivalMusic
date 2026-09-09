#pragma once
#include "../GameObject.h"
#include "../../Enum/Direction.h"
class Character : public GameObject {
protected:
	Direction direction;	//	向き

	int hp;		//	体力
	int maxHp;	//	最大体力

	int atk;	//	攻撃力
	int def;	//	防御力

	float moveSpeed;	//	移動速度
public:
	Character(VECTOR _pos = VZero, std::string _tag = "");

	virtual ~Character();

public:	//	Getterm,Setter
	inline Direction GetDirection() const { return direction; }
	inline void SetDirection(Direction _dir) { direction = _dir; }

	inline int GetHp() const { return hp; }
	inline void SetHp(int _hp) { hp = _hp; }
	inline int GetMaxHp() const { return maxHp; }
	inline void SetMaxHp(int _maxHp) { maxHp = _maxHp; }
	inline void AddHp(int _heel) {
		if(hp + _heel < maxHp)
			hp += _heel;
		else 
			hp = maxHp;
	}

	inline void SubHp(int _damage) { hp -= _damage - def; }
	void Damage(Character* _attacker, int _rawDamage);

	inline int GetAtk() const { return atk; }
	inline void SetAtk(int _atk) { atk = _atk; }

	inline int GetDef() const { return def; }
	inline void SetDef(int _def) { def = _def; }

	inline void SetPosition(VECTOR _pos) override { position = _pos; }

	bool IsDead() const { return hp <= 0; }
};

