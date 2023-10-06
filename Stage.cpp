#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <string>
#include <sstream>

using std::string;
void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    table_[_z][_x].bType = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
    table_[_z][_x].height = _height;
}

bool Stage::SaveFile()
{
    char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");     //����
    ofn.lpstrFile = fileName;               	//�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
    ofn.Flags = OFN_OVERWRITEPROMPT;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q

    //�u�t�@�C����ۑ��v�_�C�A���O
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);

    //�L�����Z�������璆�f
    if (selFile == FALSE) return false;

    HANDLE hFile;
    //�t�@�C�����J��
    hFile = CreateFile(
        fileName,  //�t�@�C����
        GENERIC_WRITE,              //�A�N�Z�X���[�h�i�������ݗp�j
        0,                          //���L�i�Ȃ��j
        NULL,                       //�Z�L�����e�B�����i�p�����Ȃ��j
        CREATE_ALWAYS,              //�쐬���@
        FILE_ATTRIBUTE_NORMAL,      //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                      //�g�������i�Ȃ��j

    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, ("CANNOT OPEN FILE: " + (string)fileName).c_str(), "Error", MB_OK);
        return false;
    }

    //�f�[�^����
    std::string writeData;
    for (int z = 0; z < Z_SIZE; z++) {
        for (int x = 0; x < X_SIZE; x++) {
            std::stringstream ss;
            ss << z << "," << x << "," << table_[z][x].height << "," << table_[z][x].bType << "\r\n";
            writeData += ss.str();
        }
    }
    writeData += "\0";

    DWORD dwBytes = 0;				//�������݈ʒu
    BOOL res = WriteFile(
        hFile,								//�t�@�C���n���h��
        writeData.c_str(),                  //�ۑ�����f�[�^�i������j
        (DWORD)strlen(writeData.c_str()),   //�������ޕ�����
        &dwBytes,							//�������񂾃T�C�Y������ϐ�
        NULL);								//�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    if (res == FALSE) {
        MessageBox(NULL, ("FAILED TO WRITE: " + (string)fileName).c_str(), "Error", MB_OK);
        return false;
    }

    //�t�@�C�������
    CloseHandle(hFile);
}

bool Stage::LoadFile()
{
    string fileName = "data.map";
    HANDLE hFile;
    //�t�@�C�����J��
    hFile = CreateFile(
        fileName.c_str(),  //�t�@�C����
        GENERIC_READ,              //�A�N�Z�X���[�h�i�������ݗp�j
        0,                          //���L�i�Ȃ��j
        NULL,                       //�Z�L�����e�B�����i�p�����Ȃ��j
        OPEN_EXISTING,              //�쐬���@
        FILE_ATTRIBUTE_NORMAL,      //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                      //�g�������i�Ȃ��j

    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, ("CANNOT OPEN FILE: " + fileName).c_str(), "Error", MB_OK);
        return false;
    }

    //�t�@�C���T�C�Y�擾
    DWORD fileSize = GetFileSize(hFile, NULL);

    //�t�@�C���̃T�C�Y�����������m��
    char* data = new char[fileSize];

    DWORD dwBytes = 0; //�ǂݍ��݈ʒu
    BOOL res = ReadFile(
        hFile,     //�t�@�C���n���h��
        data,      //�f�[�^������ϐ�
        fileSize,  //�ǂݍ��ރT�C�Y
        &dwBytes,  //�ǂݍ��񂾃T�C�Y
        NULL);     //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    if (res == FALSE) {
        MessageBox(NULL, ("FAILED TO WRITE: " + fileName).c_str(), "Error", MB_OK);
        return false;
    }

    CloseHandle(hFile);

    //�����Ɏ����Ă����f�[�^�ǂݍ��ރR�[�h

}

//�R���X�g���N�^
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

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    std::string modelName[] = {
        "BoxDefault","BoxBrick","BoxGrass","BoxSand", "BoxWater"
    };
    //���f���f�[�^�̃��[�h
    std::string ext = "fbx";
    std::string folder = "Assets/";
    for (int i = 0; i < MODEL_NUM; i++) {
        hModel_[i] = Model::Load(folder + modelName[i] + "." + ext);
        assert(hModel_[i] >= 0);
    }
    //�u���b�N�̃^�C�v���Z�b�g
    for (int z = 0; z < Z_SIZE; z++) {
        for (int x = 0; x < X_SIZE; x++) {
           SetBlock(x,z,(BLOCKTYPE)((x+z)%MODEL_NUM));
        }
    }

}

//�X�V
void Stage::Update()
{
    if (Input::IsMouseButtonDown(0)) {
        //�r���[�|�[�g�s��
        float w = (float)(Direct3D::scrWidth / 2.0f);
        float h = (float)(Direct3D::scrHeight / 2.0f);
        //offsetx,y��0
        //minZ=0 maxZ=1
        XMMATRIX vp = {
            w,  0,  0,  0,
            0, -h,  0,  0,
            0,  0,  1,  0,
            w,  h,  0,  1
        };
        //�r���[�|�[�g
        XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
        //�v���W�F�N�V�����ϊ�
        XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
        //�r���[�ϊ�
        XMMATRIX invview = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

        XMFLOAT3 mousePosFront = Input::GetMousePosition();
        mousePosFront.z = 0.0f;
        XMFLOAT3 mousePosBack = Input::GetMousePosition();
        mousePosBack.z = 1.0f;
        //mousePosFront XMVECTOR�ɕϊ�
        //�����invVP,invProj,invView��������
        //mousePosBack��XMVECTOR�ɕϊ�
        //����ɁV
        //mousePosFront����mousePosBack�ɂ����ă��C��ł� hModel_[0]
        //���C������������u���[�N�|�C���g�Ŏ~�߂�
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

//�`��
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

//�J��
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
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"�����K");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)"��");
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
        if (LOWORD(wp) == ID_MENU_SAVE) {
            SaveFile();
            break;
        }
        if (LOWORD(wp) == ID_MENU_OPEN) {
            LoadFile();
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