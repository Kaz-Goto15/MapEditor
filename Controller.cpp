#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"


//コンストラクタ
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller")
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
    
    if (Input::IsKey(DIK_A))CamTarget_.x--;



    Camera::SetPosition(CamPosition_);
    Camera::SetTarget(CamTarget_);

}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}