#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
//�R���X�g���N�^
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller"),
    movUnit_(0.1f)
{
    transform_.rotate_.x = 45.5;
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
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);                            //���݈ʒu���x�N�g���^�ɕϊ�
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));   //Y����Y��]�ʕ���]������s��
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));   //X����X��]�ʕ���]������s��
    XMVECTOR vFrontMov = { 0, 0, movUnit_, 0 };                                          //1�t���[���̈ړ��x�N�g��
    vFrontMov = XMVector3TransformCoord(vFrontMov, mRotY);                                    //�ړ��x�N�g����ό`
    XMVECTOR vRightMov = { movUnit_, 0, 0, 0 };                                          //1�t���[���̈ړ��x�N�g��
    vRightMov = XMVector3TransformCoord(vRightMov, mRotY);                                    //�ړ��x�N�g����ό`
    //XMVector3Cross(vFrontMov, XMVectorSet(0, -1, 0, 0));
    
    //�ړ�
    //Press W to Move
    if (Input::IsKey(DIK_W)) {
        vPos += vFrontMov;
    }
    //Press S to Back
    if (Input::IsKey(DIK_S)) {
        vPos -= vFrontMov;
    }
    //Press A to Left Move
    if (Input::IsKey(DIK_A)) {
        vPos -= vRightMov;
    }
    //Press D to Right Move
    if (Input::IsKey(DIK_D)) {
        vPos += vRightMov;
    }

    XMStoreFloat3(&transform_.position_, vPos); //���݈ʒu���x�N�g������transform_.position_�ɖ߂�

    //Rotate ��]
    if (Input::IsKey(DIK_LEFTARROW)) {
        transform_.rotate_.y -= 1.0f;
    }
    if (Input::IsKey(DIK_RIGHTARROW)) {
        transform_.rotate_.y += 1.0f;
    }
    if (Input::IsKey(DIK_UPARROW)) {
        transform_.rotate_.x += 1.0f;
        if (transform_.rotate_.x > 89.9f)transform_.rotate_.x = 89.9f;
    }
    if (Input::IsKey(DIK_DOWNARROW)) {
        transform_.rotate_.x -= 1.0f;
        if (transform_.rotate_.x < 0.0f)transform_.rotate_.x = 0.0f;
    }

    //�J�����ݒ� �ʒu->�Ώۂ̌��
    XMVECTOR vCam = { 0,0,-10,0 };                  //�����w��
    vCam = XMVector3TransformCoord(vCam, mRotX * mRotY);    //�ό`:��]
    Camera::SetPosition(vPos + vCam);            //�Z�b�g

    Camera::SetTarget(transform_.position_);

}

//�`��
void Controller::Draw()
{
}

//�J��
void Controller::Release()
{
}