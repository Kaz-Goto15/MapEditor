#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"


//�R���X�g���N�^
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller")
{
    CamPosition = { 0,3,-8 };
    CamTarget = { 0,2,0 };
    Camera::SetPosition(CamPosition);
    Camera::SetTarget(CamTarget);
}

//�f�X�g���N�^
Controller::~Controller()
{
}

//������
void Controller::Initialize()
{
}

//�X�V
void Controller::Update()
{
    
    if (Input::IsKey(DIK_A))CamPosition.x--;



    Camera::SetPosition(CamPosition);
    Camera::SetTarget(CamTarget);

}

//�`��
void Controller::Draw()
{
}

//�J��
void Controller::Release()
{
}