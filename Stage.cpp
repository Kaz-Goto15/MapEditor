#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "resource.h"
#include <string>

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    table_[_z][_x].bType = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
    table_[_z][_x].height = _height;
}

//コンストラクタ
Stage::Stage(GameObject* parent):
    GameObject(parent, "Stage")
{
    std::fill(hModel_, hModel_+ MODEL_NUM, -1);
    for (int z = 0; z < Z_SIZE; z++) {
        for (int x = 0; x < X_SIZE; x++) {
            SetBlock(x, z, DEFAULT);
            SetBlockHeight(x, z, 0);
        }
    }
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    std::string modelName[] = {
        "BoxDefault","BoxBrick","BoxGrass","BoxSand", "BoxWater"
    };
    //モデルデータのロード
    std::string ext = "fbx";
    std::string folder = "Assets/";
    for (int i = 0; i < MODEL_NUM; i++) {
        hModel_[i] = Model::Load(folder + modelName[i] + "." + ext);
        assert(hModel_[i] >= 0);
    }
    //ブロックのタイプをセット
    for (int z = 0; z < Z_SIZE; z++) {
        for (int x = 0; x < X_SIZE; x++) {
           SetBlock(x,z,(BLOCKTYPE)((x+z)%MODEL_NUM));
        }
    }

}

//更新
void Stage::Update()
{
    //ビューポート行列
    //float w = (float)Direct3D::scrWidth / 2.0f;
    //float h = (float)Direct3D::scrHeight / 2.0f;

    if (Input::IsKey(DIK_SPACE)) {
        for (int i = 0; i < 15; i++) {
            int xr = rand() % X_SIZE;
            int zr = rand() % Z_SIZE;
            SetBlockHeight(xr, zr, table_[zr][xr].height += 1);
        }
    }
}

//描画
void Stage::Draw()
{
    Transform stageTrans;
    for (int z = 0; z < Z_SIZE; z++) {
        stageTrans.position_.z = z;
        for (int x = 0; x < X_SIZE; x++) {
            stageTrans.position_.x = x;
            for (int y = 0; y < table_[z][x].height + 1; y++) {
                stageTrans.position_.y = y;

                Model::SetTransform(hModel_[table_[z][x].bType], stageTrans);
                Model::Draw(hModel_[table_[z][x].bType]);
            }
        }

    }

}

//開放
void Stage::Release()
{
}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"レンガ");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"草");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"砂");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"水");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_SETCURSEL, 0, 0);
        return TRUE;
    //case WM_COMMAND:
    //    SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_GETCHECK, BST_CHECKED, 0);
    }
    return FALSE;
}