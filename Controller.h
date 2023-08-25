#include "Engine/GameObject.h"

//�X�e�[�W���Ǘ�����N���X
class Controller : public GameObject
{
    XMFLOAT3 CamPosition, CamTarget;
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