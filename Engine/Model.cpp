#include "Model.h"
namespace Model {
	struct  ModelData
	{
		Fbx* pFbx_;
		Transform transform_;
		std::string fileName_;
	};
	std::vector<ModelData*> modelList_;	//モデルのポインタを入れる
}
int Model::Load(std::string fileName)
{
	ModelData* pData = new ModelData;
	pData->fileName_ = fileName;
	pData->pFbx_ = nullptr;
	//ファイル名が同じ場合、読み込みを行わない

	for (auto e : modelList_) {
		if (e->fileName_ == fileName) {
			pData->pFbx_ = e->pFbx_;
			break;
		}
	}
	if (pData->pFbx_ == nullptr) {
		pData->pFbx_ = new Fbx;
		pData->pFbx_->Load(fileName);
	}
	modelList_.push_back(pData);
	return modelList_.size() - 1;	//modelListのインデックス
}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList_[hModel]->transform_ = transform;
}

void Model::Draw(int hModel)
{
	modelList_[hModel]->pFbx_->Draw(modelList_[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false;	//参照されているか
	for (int i = 0; i < modelList_.size(); i++) {
		for (int j = i + 1; j < modelList_.size(); j++) {
			if (modelList_[i]->pFbx_ == modelList_[j]->pFbx_) {
				isReffered = true;
				break;
			}
		}
		if (!isReffered) {
			SAFE_DELETE(modelList_[i]->pFbx_);
		}
		SAFE_DELETE(modelList_[i]);
	}
	modelList_.clear();
}
