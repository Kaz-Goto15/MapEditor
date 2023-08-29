#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
//コンストラクタ
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller"),
    movUnit_(0.1f)
{
    CamPosition_ = { 0,3,-8 };
    CamTarget_ = { 0,2,0 };
    Camera::SetPosition(CamPosition_);
    Camera::SetTarget(CamTarget_);
}

//デストラクタ
Controller::~Controller()
{
}

//初期化
void Controller::Initialize()
{
}

//更新
void Controller::Update()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);                            //現在位置をベクトル型に変換
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));   //Y軸でY回転量分回転させる行列
    XMVECTOR vFrontMov = { 0, 0, movUnit_, 0 };                                          //1フレームの移動ベクトル
    XMVECTOR vRightMov = { movUnit_, 0, 0, 0 };                                          //1フレームの移動ベクトル
    vFrontMov = XMVector3TransformCoord(vFrontMov, mRotY);                                    //移動ベクトルを変形
    vRightMov = XMVector3TransformCoord(vRightMov, mRotY);                                    //移動ベクトルを変形
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
        ////ベクトルの回転(外積)
        //XMVECTOR b = XMVectorSet(0, 1, 0, 0);
        //XMVECTOR c = XMVector3Cross(vFrontMov, b);
        //vPos += c;
        vPos += vRightMov;
    }
    //Press D to Right Move
    if (Input::IsKey(DIK_D)) {
        //XMVECTOR b = XMVectorSet(0, 1, 0, 0);
        //XMVECTOR c = XMVector3Cross(vFrontMov, b);    //cは(0, 1, 0)になる

        ////移動量追加
        //vPos -= c;
        vPos -= vRightMov;
    }

    XMStoreFloat3(&transform_.position_, vPos); //現在位置をベクトルからtransform_.position_に戻す

    //Rotate 回転
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

    //カメラ設定 位置->対象の後方
    XMVECTOR vCam = { 0,5,-10,0 };                  //距離指定
    vCam = XMVector3TransformCoord(vCam, mRotY);    //変形:回転          /*XMStoreFloat3(&CamPosition_, vPos + vCam);   //XMFLOAT3変換:現在座標+回転変形後の距離 Cameraの優秀な機能でVectorのまま入れられる*/
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