#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
//�R���X�g���N�^
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller"),
    movMaxSpd_(0.15f),
    movSpd_(0)
{
    transform_.rotate_.x = 45.5;
}

//�f�X�g���N�^
Controller::~Controller()
{
}

//������
void Controller::Initialize()
{
    movcode[MV_FRONT] = DIK_W;
}

//�X�V
void Controller::Update()
{
    bool mvs = true;
    for (auto& k : movcode) {
        if (Input::IsKey(k)) {
            movSpd_ += movMaxSpd_ / 20;
            if (movSpd_ > movMaxSpd_)movSpd_ = movMaxSpd_;
            break;
        }
    }

    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);                            //���݈ʒu���x�N�g���^�ɕϊ�
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));   //Y����Y��]�ʕ���]������s��
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));   //X����X��]�ʕ���]������s��
    XMVECTOR vFrontMov = { 0, 0, movSpd_, 0 };                                          //1�t���[���̈ړ��x�N�g��
    vFrontMov = XMVector3TransformCoord(vFrontMov, mRotY);                                    //�ړ��x�N�g����ό`
    XMVECTOR vRightMov = { movSpd_, 0, 0, 0 };                                          //1�t���[���̈ړ��x�N�g��
    vRightMov = XMVector3TransformCoord(vRightMov, mRotY);                                    //�ړ��x�N�g����ό`
    XMVECTOR vUpMov = { 0, movSpd_, 0, 0 };                                          //1�t���[���̈ړ��x�N�g��
    vUpMov = XMVector3TransformCoord(vUpMov, mRotY);                                    //�ړ��x�N�g����ό`
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
    //�ǉ�����
    //Press Q to Up
    if (Input::IsKey(DIK_Q)) {
        vPos += vUpMov;
    }
    //Press E to Down
    if (Input::IsKey(DIK_E)) {
        vPos -= vUpMov;
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