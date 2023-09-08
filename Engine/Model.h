#pragma once
#include <string>
#include <vector>
#include "Transform.h"
#include "Fbx.h"

namespace Model
{
	//struct ModelData {
	//	Fbx* pFbx_;
	//	Transform transform_;
	//	std::string fileName_;
	//};
	int Load(std::string fileName);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void Release();
	void RayCast(int hModel, RayCastData& rayData);
};

