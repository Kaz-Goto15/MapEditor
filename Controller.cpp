#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"


//�R���X�g���N�^
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller")
{
    CamPosition_ = { 0,3,-8 };
    CamTarget_ = { 0,2,0 };
    Camera::SetPosition(CamPosition_);
    Camera::SetTarget(CamTarget_);
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
    
    if (Input::IsKey(DIK_A))CamTarget_.x--;



    Camera::SetPosition(CamPosition_);
    Camera::SetTarget(CamTarget_);

}

//�`��
void Controller::Draw()
{
}

//�J��
void Controller::Release()
{
}