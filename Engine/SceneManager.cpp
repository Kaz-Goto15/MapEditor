#include "SceneManager.h"
#include "Model.h"
#include "../TestScene.h"

SceneManager::SceneManager(GameObject* parent):GameObject(parent, "SceneManager")
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TEST;
	nextSceneID_ = currentSceneID_;
	Instantiate<TestScene>(this);
}

void SceneManager::Update()
{
	if (currentSceneID_ != nextSceneID_) {
		auto scene = childList_.begin();
		(*scene)->ReleaseSub();
		SAFE_DELETE(*scene);
		childList_.clear();
		Model::Release();

		switch (nextSceneID_) {
		case SCENE_ID_TEST:
			Instantiate<TestScene>(this); break;
		default:
			break;
		}
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

/// <summary>
/// 指定したシーンをnextSceneID_に設定
/// </summary>
/// <param name="_next">次のシーン</param>
void SceneManager::ChangeScene(SCENE_ID _next)
{//シーン切り替え
	//現在のｓ－インと現在１シーンは切り替える
	nextSceneID_ = _next;
}
