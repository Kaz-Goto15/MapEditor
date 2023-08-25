#include "Stage.h"
#include "Engine/Model.h"

//�R���X�g���N�^
Stage::Stage(GameObject* parent):
    GameObject(parent, "Stage"),
    hModel_(-1)
{
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets/BoxDefault.fbx");
    assert(hModel_ >= 0);
}

//�X�V
void Stage::Update()
{
}

//�`��
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

//�J��
void Stage::Release()
{
}