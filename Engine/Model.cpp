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

void Model::RayCast(int hModel, RayCastData& rayData)
{
	//0.モデルトランスフォームをカリキュレーション
	//1.ワールド行列の逆行列
	//2.レイ通過点を求める(モデル空間でのレイの方向ベクトルを求める)
	//3.（rayData.staertをモデル空間に変換(1.をかける)
	//4.視点から方向ベクトルを少しのばした先）通過点（2.）に1.をかける
	//5.rayData.dirを3.から4.に向かうベクトルにする(引き算)
	modelList_[hModel]->transform_.Calclation();
	XMMATRIX wInv = XMMatrixInverse(nullptr, modelList_[hModel]->transform_.GetWorldMatrix());
	XMVECTOR vPass = {
		rayData.start.x + rayData.dir.x,
		rayData.start.y + rayData.dir.y,
		rayData.start.z + rayData.dir.z,
		rayData.start.w + rayData.dir.w
	};
	XMVECTOR vStart = XMLoadFloat4(&rayData.start);
	vStart = XMVector3TransformCoord(vStart, wInv);	//w要素無視
	XMStoreFloat4(&rayData.start, vStart); //値をrayDatに戻す
	vPass = XMVector3TransformCoord(vPass, wInv);	//w要素無視
	XMStoreFloat4(&rayData.dir, vPass - vStart );

	modelList_[hModel]->pFbx_->RayCast(rayData);
}
