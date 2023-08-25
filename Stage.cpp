#include "Stage.h"
#include "Engine/Model.h"

//コンストラクタ
Stage::Stage(GameObject* parent):
    GameObject(parent, "Stage"),
    hModel_(-1)
{
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/BoxDefault.fbx");
    assert(hModel_ >= 0);
}

//更新
void Stage::Update()
{
}

//描画
void Stage::Draw()
{
    Transform stageTrans;

    //Model::SetTransform(hModel_, transform_);
    int stageRange = 15;
    for (int z = 0; z < stageRange; z++) {
        stageTrans.position_.z = z;
        for (int x = 0; x < stageRange; x++) {
            stageTrans.position_.x = x;
            Model::SetTransform(hModel_, stageTrans);
            Model::Draw(hModel_);
        }

    }
    Model::Draw(hModel_);

}

//開放
void Stage::Release()
{
}