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
    //int table_[Z_SIZE][X_SIZE];

    struct {
        BLOCKTYPE bType;
        int height;
    }table_[Z_SIZE][X_SIZE];
public:
    //�R���X�g���N�^
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);
};