#include "Stage.h"
#include "Engine/Model.h"

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    table_[_x][_z] = _type;
}

//�R���X�g���N�^
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

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    std::string modelName[] = {
        "BoxDefault","BoxBrick","BoxGrass","BoxSand", "BoxWater"
    };
    //���f���f�[�^�̃��[�h
    std::string ext = "fbx";
    std::string folder = "Assets/";
    for (int i = 0; i < MODEL_NUM; i++) {
        hModel_[i] = Model::Load(folder + modelName[i] + "." + ext);
        assert(hModel_[i] >= 0);
    }
    //�u���b�N�̃^�C�v���Z�b�g
    for (int x = 0; x < X_SIZE; x++) {
        for (int z = 0; z < Z_SIZE; z++) {
            table_[x][z] = (x+z)%MODEL_NUM;
        }
    }

}

//�X�V
void Stage::Update()
{
}

//�`��
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

//�J��
void Stage::Release()
{
}