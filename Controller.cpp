#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"

//�R���X�g���N�^
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller"),
    movMaxSpd_(0.2f),
    movSpd_(0),
    keyMovAcc_(30.0f),
    keyMovDec_(20.0f),
    mouseMovReg_(0.5f)
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
    movcode[MV_BACK] = DIK_S;
    movcode[MV_LEFT] = DIK_A;
    movcode[MV_RIGHT] = DIK_D;
    movcode[MV_UP] = DIK_Q;
    movcode[MV_DOWN] = DIK_E;
}

//�X�V
void Controller::Update()
{
    if (enMovInertia_) {
    }
    else {

    }
    //move
    for (int k = 0; k < MV_MAX; k++) {
        if (Input::IsKey(movcode[k])) {
            movSpdRotate_[k] += movMaxSpd_ / keyMovAcc_;
            if (movSpdRotate_[k] > movMaxSpd_)movSpdRotate_[k] = movMaxSpd_;
        }
        else {
            movSpdRotate_[k] -= movMaxSpd_ / keyMovDec_;
            if (movSpdRotate_[k] < 0)movSpdRotate_[k] = 0;
        }
    }

    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);                            //���݈ʒu���x�N�g���^�ɕϊ�
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));   //Y����Y��]�ʕ���]������s��
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));   //X����X��]�ʕ���]������s��

    XMFLOAT3 fMov = {
    movSpdRotate_[MV_RIGHT] - movSpdRotate_[MV_LEFT],
    movSpdRotate_[MV_UP] - movSpdRotate_[MV_DOWN],
    movSpdRotate_[MV_FRONT] - movSpdRotate_[MV_BACK]
    };
    XMVECTOR vMov = XMLoadFloat3(&fMov);
    //vMov *= 0.2f;
    vMov = XMVector3TransformCoord(vMov, mRotY);
    vPos += vMov;
    XMStoreFloat3(&transform_.position_, vPos); //���݈ʒu���x�N�g������transform_.position_�ɖ߂�

    std::string resStr = std::to_string(Input::GetMouseMove().x) + ", " + std::to_string(Input::GetMouseMove().y) + "\n";
    OutputDebugString(resStr.c_str());
    //Rotate ��]
    if (Input::IsMouseButton(1)) {
        XMFLOAT3 mouseMov = Input::GetMouseMove();
        transform_.rotate_.y += mouseMov.x * mouseMovReg_;
        transform_.rotate_.x += mouseMov.y * mouseMovReg_;
        if (transform_.rotate_.x > 89.9f)transform_.rotate_.x = 89.9f;
        if (transform_.rotate_.x < 0.0f)transform_.rotate_.x = 0.0f;

    }

    //�J�����ݒ� �ʒu->�Ώۂ̌��
    XMVECTOR vCam = { 0,0,-10,0 };                  //�����w��
    vCam = XMVector3TransformCoord(vCam, mRotX * mRotY);    //�ό`:��]
    Camera::SetPosition(vPos + vCam);            //�Z�b�g

    Camera::SetTarget(transform_.position_);

    /*
    movSpd_ = movMaxSpd_;
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
    //vFrontMov *= 0.2f;

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

    */
}

//�`��
void Controller::Draw()
{
}

//�J��
void Controller::Release()
{
}

/*
BOOL Controller::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, IDC_CHECK_INERTIA), BM_SETCHECK, BST_CHECKED, 0);
        enMovInertia_ = true;
        SendMessage(GetDlgItem(hDlg, IDC_CHECK_ENABLE_MOUSE_ROTATE), BM_SETCHECK, BST_CHECKED, 0);
        canMouseCtlRotate_ = true;
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wp) == IDC_CHECK_INERTIA) {
            enMovInertia_ = SendMessage(GetDlgItem(hDlg, IDC_CHECK_INERTIA), BM_GETCHECK, 0, 0);
            break;
        }
        if (LOWORD(wp) == IDC_CHECK_ENABLE_MOUSE_ROTATE) {
            canMouseCtlRotate_ = SendMessage(GetDlgItem(hDlg, IDC_CHECK_ENABLE_MOUSE_ROTATE), BM_GETCHECK, 0, 0);
            break;
        }
    }
    return FALSE;
}
*/