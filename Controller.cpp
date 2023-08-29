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
    
    //戦車の現在位置をベクトル型に変換
    XMVECTOR nowVec = XMLoadFloat3(&transform_.position_);
    //1フレームの移動ベクトル
    XMVECTOR movVec = { 0, 0, movUnit_, 0 };
    //Y軸でY回転量分回転させる行列
    XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    //移動ベクトルを変形
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
        //ベクトルの回転(外積)
        XMVECTOR b = XMVectorSet(0, 1, 0, 0);
        XMVECTOR c = XMVector3Cross(movVec, b);
        nowVec += c;

        //現在位置をベクトルからtransform_.position_に戻す
        XMStoreFloat3(&transform_.position_, nowVec);
    }
    //Press D to Right Move
    if (Input::IsKey(DIK_D)) {
        XMVECTOR b = XMVectorSet(0, 1, 0, 0);
        XMVECTOR c = XMVector3Cross(movVec, b);    //cは(0, 1, 0)になる

        //移動量追加
        nowVec -= c;
        //現在位置をベクトルからtransform_.position_に戻す
        XMStoreFloat3(&transform_.position_, nowVec);
    }

    //if (Input::IsKey(DIK_A))CamTarget_.x--;

    Camera::SetTarget(transform_.position_);
    //カメラ位置->対象の後方
    XMVECTOR vCam = { 0,5,-10,0 };                           //距離指定
    vCam = XMVector3TransformCoord(vCam, rotMat);   //変形:回転
    XMStoreFloat3(&CamPosition_, nowVec + vCam);          //XMFLOAT3変換:現在座標+回転変形後の距離
    Camera::SetPosition(CamPosition_);                    //セット

    //Camera::SetPosition(CamPosition_);
    //Camera::SetTarget(CamTarget_);

}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}