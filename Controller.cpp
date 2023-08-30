#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
//コンストラクタ
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller"),
    movMaxSpd_(0.15f),
    movSpd_(0)
{
    transform_.rotate_.x = 45.5;
}

//デストラクタ
Controller::~Controller()
{
}

//初期化
void Controller::Initialize()
{
    movcode[MV_FRONT] = DIK_W;
}

//更新
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

    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);                            //現在位置をベクトル型に変換
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));   //Y軸でY回転量分回転させる行列
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));   //X軸でX回転量分回転させる行列
    XMVECTOR vFrontMov = { 0, 0, movSpd_, 0 };                                          //1フレームの移動ベクトル
    vFrontMov = XMVector3TransformCoord(vFrontMov, mRotY);                                    //移動ベクトルを変形
    XMVECTOR vRightMov = { movSpd_, 0, 0, 0 };                                          //1フレームの移動ベクトル
    vRightMov = XMVector3TransformCoord(vRightMov, mRotY);                                    //移動ベクトルを変形
    XMVECTOR vUpMov = { 0, movSpd_, 0, 0 };                                          //1フレームの移動ベクトル
    vUpMov = XMVector3TransformCoord(vUpMov, mRotY);                                    //移動ベクトルを変形
    //XMVector3Cross(vFrontMov, XMVectorSet(0, -1, 0, 0));
    
    //移動
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
    //追加した
    //Press Q to Up
    if (Input::IsKey(DIK_Q)) {
        vPos += vUpMov;
    }
    //Press E to Down
    if (Input::IsKey(DIK_E)) {
        vPos -= vUpMov;
    }

    XMStoreFloat3(&transform_.position_, vPos); //現在位置をベクトルからtransform_.position_に戻す

    //Rotate 回転
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

    //カメラ設定 位置->対象の後方
    XMVECTOR vCam = { 0,0,-10,0 };                  //距離指定
    vCam = XMVector3TransformCoord(vCam, mRotX * mRotY);    //変形:回転
    Camera::SetPosition(vPos + vCam);            //セット

    Camera::SetTarget(transform_.position_);

}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}