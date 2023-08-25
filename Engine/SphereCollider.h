#pragma once
class SphereCollider
{
	float radius_;
public:
	SphereCollider(float _r);
	float GetRadius() { return radius_; }
};

