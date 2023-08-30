#include "Engine/GameObject.h"
#include <dinput.h>

//�X�e�[�W���Ǘ�����N���X
class Controller : public GameObject
{
    enum KEY {
        MV_FRONT = DIK_W,
        MV_BACK = DIK_S,
        MV_LEFT = DIK_A,
        MV_RIGHT = DIK_D,
        MV_UP = DIK_Q,
        MV_DOWN = DIK_E,
        MV_MAX
    };
    int movcode[MV_MAX];
    float movSpd_, movMaxSpd_;
public:
    //�R���X�g���N�^
    Controller(GameObject* parent);

    //�f�X�g���N�^
    ~Controller();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};