#include "Engine/GameObject.h"
#include "resource.h"
#include <vector>

namespace {
    const int MODEL_NUM = 5;
    const int X_SIZE = 15;
    const int Z_SIZE = 15;
    enum BLOCKTYPE {
        DEFAULT,
        BRICK,
        GRASS,
        SAND,
        WATER
    };
    enum MODE {
        UP = IDC_RADIO_UP,
        DOWN = IDC_RADIO_DOWN,
        CHANGE= IDC_RADIO_CHANGE,
    };
}
//�X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_[MODEL_NUM];    //���f���ԍ�
    struct {
        BLOCKTYPE bType;
        int height;
    }table_[Z_SIZE][X_SIZE];

    int mode_;      //0:������ 1:������ 2:��ރJ�G��
    int select_;    //���
    bool isEdited;
    void NewFile();
    bool SaveFile();
    bool LoadFile();
    bool DestructContent();
    /// <summary>
    /// �Ǎ��t�@�C������f�[�^���i�[
    /// </summary>
    /// <param name="_fileSize">�t�@�C���T�C�Y</param>
    /// <param name="_puredata">���f�[�^������</param>
    /// <param name="_data">�f�[�^�i�[�z��</param>
    void LoadData(DWORD _fileSize, char* _puredata, std::vector<std::vector<std::string>>* _data);
    void GetSingleData(std::string* result, std::string data, DWORD* index);
public:
    //�R���X�g���N�^
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Set();
    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);

    //�U�v���V�[�W��
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};