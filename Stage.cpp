#include "Stage.h"
#include "Engine/Model.h"

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    table_[_x][_z] = _type;
}

//コンストラクタ
Stage::Stage(GameObject* parent):
    GameObject(parent, "Stage")
{
    std::fill(hModel_, hModel_+ MODEL_NUM, -1);
    for (int x = 0; x < X_SIZE; x++) {
        for (int z = 0; z < Z_SIZE; z++) {
            table_[x][z] = -1;
        }
    }
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    std::string modelName[] = {
        "BoxDefault","BoxBrick","BoxGrass","BoxSand", "BoxWater"
    };
    //モデルデータのロード
    std::string ext = "fbx";
    std::string folder = "Assets/";
    for (int i = 0; i < MODEL_NUM; i++) {
        hModel_[i] = Model::Load(folder + modelName[i] + "." + ext);
        assert(hModel_[i] >= 0);
    }
    //ブロックのタイプをセット
    for (int x = 0; x < X_SIZE; x++) {
        for (int z = 0; z < Z_SIZE; z++) {
            table_[x][z] = (x+z)%MODEL_NUM;
        }
    }

}

//更新
void Stage::Update()
{
}

//描画
void Stage::Draw()
{
    Transform stageTrans;
    for (int z = 0; z < Z_SIZE; z++) {
        stageTrans.position_.z = z;
        for (int x = 0; x < X_SIZE; x++) {
            stageTrans.position_.x = x;
            Model::SetTransform(hModel_[table_[x][z]], stageTrans);
            Model::Draw(hModel_[table_[x][z]]);
        }

    }

}

//開放
void Stage::Release()
{
}