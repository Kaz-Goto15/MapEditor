#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
//�R���X�g���N�^
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller"),
    movUnit_(0.1f)
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
    
    //��Ԃ̌��݈ʒu���x�N�g���^�ɕϊ�
    XMVECTOR nowVec = XMLoadFloat3(&transform_.position_);
    //1�t���[���̈ړ��x�N�g��
    XMVECTOR movVec = { 0, 0, movUnit_, 0 };
    //Y����Y��]�ʕ���]������s��
    XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    //�ړ��x�N�g����ό`
    movVec = XMVector3TransformCoord(movVec, rotMat);

    //Rotate
    if (Input::IsKey(DIK_LEFTARROW)) {
        transform_.rotate_.y -= 1.0f;
    }
    if (Input::IsKey(DIK_RIGHTARROW)) {
        transform_.rotate_.y += 1.0f;
    }
    //if (Input::IsKey(DIK_UPARROW)) {
    //    XMFLOAT3  nrot = transform_.rotate_;
    //    transform_.rotate_.x -= 1.0f;
    //}
    //if (Input::IsKey(DIK_DOWNARROW)) {
    //    transform_.rotate_.x += 1.0f;
    //}

    //Press W to Move
    if (Input::IsKey(DIK_W)) {
        nowVec += movVec;
        XMStoreFloat3(&transform_.position_, nowVec);
    }
    //Press S to Back
    if (Input::IsKey(DIK_S)) {
        nowVec -= movVec;
        XMStoreFloat3(&transform_.position_, nowVec);
    }
    //Press A to Left Move
    if (Input::IsKey(DIK_A)) {
        //�x�N�g���̉�](�O��)
        XMVECTOR b = XMVectorSet(0, 1, 0, 0);
        XMVECTOR c = XMVector3Cross(movVec, b);
        nowVec += c;

        //���݈ʒu���x�N�g������transform_.position_�ɖ߂�
        XMStoreFloat3(&transform_.position_, nowVec);
    }
    //Press D to Right Move
    if (Input::IsKey(DIK_D)) {
        XMVECTOR b = XMVectorSet(0, 1, 0, 0);
        XMVECTOR c = XMVector3Cross(movVec, b);    //c��(0, 1, 0)�ɂȂ�

        //�ړ��ʒǉ�
        nowVec -= c;
        //���݈ʒu���x�N�g������transform_.position_�ɖ߂�
        XMStoreFloat3(&transform_.position_, nowVec);
    }

    //if (Input::IsKey(DIK_A))CamTarget_.x--;

    Camera::SetTarget(transform_.position_);
    //�J�����ʒu->�Ώۂ̌��
    XMVECTOR vCam = { 0,5,-10,0 };                           //�����w��
    vCam = XMVector3TransformCoord(vCam, rotMat);   //�ό`:��]
    XMStoreFloat3(&CamPosition_, nowVec + vCam);          //XMFLOAT3�ϊ�:���ݍ��W+��]�ό`��̋���
    Camera::SetPosition(CamPosition_);                    //�Z�b�g

    //Camera::SetPosition(CamPosition_);
    //Camera::SetTarget(CamTarget_);

}

//�`��
void Controller::Draw()
{
}

//�J��
void Controller::Release()
{
}