#include "Engine/GameObject.h"
#include "resource.h"
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
    enum MODE {
        UP = IDC_RADIO_UP,
        DOWN = IDC_RADIO_DOWN,
        CHANGE= IDC_RADIO_CHANGE,
    };
}
//�X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_[MODEL_NUM];    //���f���ԍ�
    struct {
        BLOCKTYPE bType;
        int height;
    }table_[Z_SIZE][X_SIZE];

    int mode_;      //0:������ 1:������ 2:��ރJ�G��
    int select_;    //���

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

    //�U�v���V�[�W��
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};