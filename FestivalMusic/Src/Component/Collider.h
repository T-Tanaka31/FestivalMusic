#pragma once
#include <DxLib.h>
#include "../GameObject/GameObject.h"
#include <vector>
#include "../Enum/ColliderType.h"

class Collider {
protected:
	bool isEnable;
	GameObject* pGameObject;
	VECTOR offset;

public:
	Collider(GameObject* _pObj);

	virtual ~Collider() = default;

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual bool IsHit(Collider* other) = 0;
public:
	inline bool IsEnable() const { return isEnable; }
	inline void SetEnable(bool _v) { isEnable = _v; }

	inline GameObject* GetGameObject() const { return pGameObject; }

	inline void SetOffset(VECTOR _o) { offset = _o; }

	virtual ColliderType GetType() const = 0;
};

class CircleCollider : public Collider {
private:
	float radius;
	VECTOR center;
public:
	CircleCollider(GameObject* _pObj, float _r);
	~CircleCollider() = default;

public:
	void Update();
	void Render();

	bool IsHit(Collider* other) override;
public:
	inline VECTOR GetCenter() const { return center; }
	inline float GetRadius() const { return radius; }

	inline ColliderType GetType() const override { return ColliderType::Circle; }
};

class SquareCollider : public Collider {
private:
	VECTOR size;
	VECTOR minPoint;
	VECTOR maxPoint;

public:
	SquareCollider(GameObject* obj,VECTOR _size);
	~SquareCollider() = default;

public:
	void Update() override;
	void Render() override;

	bool IsHit(Collider* other) override;

public:
	inline VECTOR GetMaxPoint() const { return maxPoint; }
	inline VECTOR GetMinPoint() const { return minPoint; }

	inline ColliderType GetType() const override { return ColliderType::Square; }
};

