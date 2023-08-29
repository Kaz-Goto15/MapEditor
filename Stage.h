#include "Engine/GameObject.h"

namespace {
    const int MODEL_NUM = 5;
    const int X_SIZE = 15;
    const int Z_SIZE = 15;
    enum BLOCKTYPE {
        DEFAULT,
        BRICK,
        GRASS,
        SAND,
        WATER
    };
}
//�X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_[MODEL_NUM];    //���f���ԍ�
    int table_[X_SIZE][Z_SIZE];
public:
    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    //�R���X�g���N�^
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};