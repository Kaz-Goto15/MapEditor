#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    table_[_z][_x].bType = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
    table_[_z][_x].height = _height;
}

//�R���X�g���N�^
Stage::Stage(GameObject* parent):
    GameObject(parent, "Stage")
{
    std::fill(hModel_, hModel_+ MODEL_NUM, -1);
    for (int z = 0; z < Z_SIZE; z++) {
        for (int x = 0; x < X_SIZE; x++) {
            SetBlock(x, z, (BLOCKTYPE)NULL);
            SetBlockHeight(x, z, 1);
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
    for (int z = 0; z < Z_SIZE; z++) {
        for (int x = 0; x < X_SIZE; x++) {
           SetBlock(x,z,(BLOCKTYPE)((x+z)%MODEL_NUM));
        }
    }

}

//�X�V
void Stage::Update()
{
    if (Input::IsKey(DIK_SPACE)) {
        for (int i = 0; i < 15; i++) {
            int xr = rand() % X_SIZE;
            int zr = rand() % Z_SIZE;
            SetBlockHeight(xr, zr, table_[zr][xr].height += 1);
        }
    }
}

//�`��
void Stage::Draw()
{
    Transform stageTrans;
    for (int z = 0; z < Z_SIZE; z++) {
        stageTrans.position_.z = z;
        for (int x = 0; x < X_SIZE; x++) {
            stageTrans.position_.x = x;
            for (int y = 0; y < table_[z][x].height; y++) {
                stageTrans.position_.y = y;

                Model::SetTransform(hModel_[table_[z][x].bType], stageTrans);
                Model::Draw(hModel_[table_[z][x].bType]);
            }
        }

    }

}

//�J��
void Stage::Release()
{
}