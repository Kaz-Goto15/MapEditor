#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
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
    if (Input::IsMouseButtonDown(0)) {
        //ビューポート行列
        float w = (float)(Direct3D::scrWidth / 2.0f);
        float h = (float)(Direct3D::scrHeight / 2.0f);
        //offsetx,yは0
        //minZ=0 maxZ=1
        XMMATRIX vp = {
            w,  0,  0,  0,
            0, -h,  0,  0,
            0,  0,  1,  0,
            w,  h,  0,  1
        };
        //ビューポート
        XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
        //プロジェクション変換
        XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
        //ビュー変換
        XMMATRIX invview = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

        XMFLOAT3 mousePosFront = Input::GetMousePosition();
        mousePosFront.z = 0.0f;
        XMFLOAT3 mousePosBack = Input::GetMousePosition();
        mousePosBack.z = 1.0f;
        //mousePosFront XMVECTORに変換
        //それにinvVP,invProj,invViewをかける
        //mousePosBackをXMVECTORに変換
        //それに〃
        //mousePosFrontからmousePosBackにかけてレイを打つ hModel_[0]
        //レイが当たったらブレークポイントで止める
        XMVECTOR vMousePosFront = XMLoadFloat3(&mousePosFront);
        vMousePosFront = XMVector3TransformCoord(vMousePosFront, invVP * invProj * invview);
        XMVECTOR vMousePosBack = XMLoadFloat3(&mousePosBack);
        vMousePosBack = XMVector3TransformCoord(vMousePosBack, invVP * invProj * invview);

        float shortestDist = -1;
        int changeTile[2]{ 0 };
        for (int z = 0; z < Z_SIZE; z++) {
            for (int x = 0; x < X_SIZE; x++) {
                for (int y = 0; y < table_[z][x].height + 1; y++) {
                    RayCastData data;
                    XMStoreFloat4(&data.start, vMousePosFront);
                    XMStoreFloat4(&data.dir, vMousePosBack - vMousePosFront);
                    Transform trans;
                    trans.position_.x = x;
                    trans.position_.y = y;
                    trans.position_.z = z;
                    Model::SetTransform(hModel_[0], trans);
                    //Model::SetTransform(hModel[table_[z][x].bType].trans);
                    Model::RayCast(hModel_[0], data);
                    if (data.hit) {
                        std::string resStr = "Hit: " + std::to_string(z) + ", " + std::to_string(x) + "\n";
                        OutputDebugString(resStr.c_str());
                        if (shortestDist == -1 || data.dist < shortestDist) {
                            shortestDist = data.dist;
                            changeTile[0] = x;
                            changeTile[1] = z;
                        }
                    }
                }
            }
        }
        std::string resStr = "change: " + std::to_string(changeTile[1]) + ", " + std::to_string(changeTile[0]) + "\n";
        OutputDebugString(resStr.c_str());
        if (shortestDist != -1) {
            switch (mode_) {
            case MODE::UP:
                SetBlockHeight(changeTile[0], changeTile[1], table_[changeTile[1]][changeTile[0]].height += 1);
                break;
            case MODE::DOWN:
                if(table_[changeTile[1]][changeTile[0]].height > 0)
                SetBlockHeight(changeTile[0], changeTile[1], table_[changeTile[1]][changeTile[0]].height -= 1);
                break;
            case MODE::CHANGE:
                SetBlock(changeTile[0], changeTile[1], (BLOCKTYPE)select_);
            }
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
        //SendMessage(GetDlgItem(hDlg, IDC_CHECK_INERTIA), BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
        mode_ = MODE::UP;
        a = true;
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"レンガ");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"草");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"砂");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"水");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_SETCURSEL, 0, 0);
        return TRUE;
    case WM_COMMAND:
        if (SendMessage(GetDlgItem(hDlg, LOWORD(wp)), BM_GETCHECK, 0, 0) == BST_CHECKED) {
            mode_ = LOWORD(wp);
        }
        if (LOWORD(wp) == IDC_COMBO3) {
            select_ = SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_GETCURSEL, 0, 0);
                break;
        }
        //switch (LOWORD(wp)) {

        //case IDC_RADIO_CHANGE:
        //    if (SendMessage(GetDlgItem(hDlg, IDC_RADIO_CHANGE), BM_GETCHECK, 0, 0) == BST_CHECKED) {
        //        mode_ = LOWORD(wp);
        //    }
        //    break;
        //case IDC_COMBO3:
        //    select_ = SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_GETCURSEL, 0, 0);
        //    break;
        //}
    }
    return FALSE;
}