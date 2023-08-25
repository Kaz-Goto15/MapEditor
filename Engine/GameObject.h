#pragma once

#include <list>
#include <string>
#include "Transform.h"
#include "Direct3D.h"

class SphereCollider;

class GameObject
{
private:
	bool isDead;

protected:
	std::list<GameObject*>	childList_;
	Transform				transform_;
	GameObject*				pParent_;
	std::string				objectName_;
	SphereCollider* pCollider_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();

	void KillMe();

	void SetTransform(Transform _tra);
	void SetPosition(XMFLOAT3 _pos) { transform_.position_ = _pos; }
	void SetRotate(XMFLOAT3 _rot) { transform_.rotate_ = _rot; }
	void SetScale(XMFLOAT3 _sca) { transform_.scale_ = _sca; }

	GameObject* GetRootJob();
	GameObject* FindChildObject(std::string _objName);
	GameObject* FindObject(std::string _objName){ return GetRootJob()->FindChildObject(_objName); }
	template <class T>
	GameObject* Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		childList_.push_back(p);
		return p;
	}

	void AddCollider(SphereCollider* pCollider);
	void Collision(GameObject* pTarget);
	virtual void OnCollision(GameObject* pTarget) {};
	void RoundRobin(GameObject* pTarget);

	std::string GetObjectName() { 
		return objectName_; 
	}
};