#include "Engine/GameObject.h"
#include <dinput.h>

//�X�e�[�W���Ǘ�����N���X
class Controller : public GameObject
{
    enum KEY {
        MV_FRONT = 0,
        MV_BACK,
        MV_LEFT,
        MV_RIGHT,
        MV_UP ,
        MV_DOWN,
        MV_MAX
    };
    int movcode[MV_MAX]{ 0 };
    float movSpdRotate_[MV_MAX]{ 0 };
    float movSpd_, movMaxSpd_;
    float mouseMovReg_, keyMovReg_;
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