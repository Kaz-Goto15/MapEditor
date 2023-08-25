#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"


//コンストラクタ
Controller::Controller(GameObject* parent) :
    GameObject(parent, "Controller")
{
    CamPosition = { 0,3,-8 };
    CamTarget = { 0,2,0 };
    Camera::SetPosition(CamPosition);
    Camera::SetTarget(CamTarget);
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
    
    if (Input::IsKey(DIK_A))CamPosition.x--;



    Camera::SetPosition(CamPosition);
    Camera::SetTarget(CamTarget);

}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}