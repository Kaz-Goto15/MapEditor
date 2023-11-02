#include "TestScene.h"
#include "Engine/SceneManager.h"
//#include "Engine/Input.h"
#include "Controller.h"
#include "Stage.h"
TestScene::TestScene(GameObject* parent):GameObject(parent, "TestScene")
{
}

void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Controller>(this);
}

void TestScene::Update() {}
void TestScene::Draw(){}
void TestScene::Release(){}
