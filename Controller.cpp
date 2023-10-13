#include "Controller.h"
#include "Stage.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"

//コンストラクタ
Controller::Controller(GameObject* parent) :
	GameObject(parent, "Controller"),
	movMaxSpd_(0.2f),
	movSpd_(0),
	keyMovAcc_(30.0f),
	keyMovDec_(20.0f),
	mouseMovReg_(0.5f),
	camDistance(10.0f),
	maxCamDist(100.0f),
	minCamDist(2.5f),
	camDistUnit(2.0f),
	camDistReg(0.05f),
	modifyMode(MD_DEFAULT)
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
	movcode[MV_BACK] = DIK_S;
	movcode[MV_LEFT] = DIK_A;
	movcode[MV_RIGHT] = DIK_D;
	movcode[MV_UP] = DIK_Q;
	movcode[MV_DOWN] = DIK_E;
}

//更新
void Controller::Update()
{
	//ヤケクソ実装
	modifyMode = MD_DEFAULT;
	if (Input::IsKey(MD_ALT))modifyMode = MD_ALT;
	//if (Input::IsKey(MD_CTRL))modifyMode = MD_CTRL;
	switch (modifyMode) {
	case MD_ALT:
		//Rotate(MOD+Mouse) / Scale(Mouse+MOD)
		//rotate(MouseL)
		if (Input::IsMouseButton(0)) {
			transform_.rotate_.y += Input::GetMouseMove().x * mouseMovReg_;
			transform_.rotate_.x += Input::GetMouseMove().y * mouseMovReg_;
			if (transform_.rotate_.x > 89.9f)transform_.rotate_.x = 89.9f;
			if (transform_.rotate_.x < 0.0f)transform_.rotate_.x = 0.0f;
		}
		//Scale(MouseR)
		if (Input::IsMouseButton(1)) {
			XMFLOAT3 mouseMov = Input::GetMouseMove();
			camDistance += -(mouseMov.x + mouseMov.y) * camDistReg;
		}
		break;
	default:
		//Set(MouseL)
		if (Input::IsMouseButtonUp(0)) {
			(Stage)(FindObject("Stage"))->Set();
		}
		break;
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

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);                            //現在位置をベクトル型に変換
	XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));   //Y軸でY回転量分回転させる行列
	XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));   //X軸でX回転量分回転させる行列

	XMFLOAT3 fMov = {
	movSpdRotate_[MV_RIGHT] - movSpdRotate_[MV_LEFT],
	movSpdRotate_[MV_UP] - movSpdRotate_[MV_DOWN],
	movSpdRotate_[MV_FRONT] - movSpdRotate_[MV_BACK]
	};
	XMVECTOR vMov = XMLoadFloat3(&fMov);
	vMov = XMVector3TransformCoord(vMov, mRotY);
	vPos += vMov;
	XMStoreFloat3(&transform_.position_, vPos); //現在位置をベクトルからtransform_.position_に戻す

	//Rotate(Key)
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

	//Scale(MouseWheel)
	if (Input::GetMouseMove().z > 0)camDistance -= camDistUnit;
	if (Input::GetMouseMove().z < 0)camDistance += camDistUnit;

	if (camDistance < minCamDist)camDistance = minCamDist;
	if (camDistance > maxCamDist)camDistance = maxCamDist;

	//カメラ設定 位置->対象の後方
	XMVECTOR vCam = { 0,0,-camDistance,0 };                  //距離指定
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