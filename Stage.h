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
//ステージを管理するクラス
class Stage : public GameObject
{
    int hModel_[MODEL_NUM];    //モデル番号
    struct {
        BLOCKTYPE bType;
        int height;
    }table_[Z_SIZE][X_SIZE];

    int mode_;      //0:あげる 1:さげる 2:種類カエル
    int select_;    //種類
    bool isEdited;
    void NewFile();
    bool SaveFile();
    bool LoadFile();
    bool DestructContent();
    /// <summary>
    /// 読込ファイルからデータを格納
    /// </summary>
    /// <param name="_fileSize">ファイルサイズ</param>
    /// <param name="_puredata">元データ文字列</param>
    /// <param name="_data">データ格納配列</param>
    void LoadData(DWORD _fileSize, char* _puredata, std::vector<std::vector<std::string>>* _data);
    void GetSingleData(std::string* result, std::string data, DWORD* index);

    enum DIRECTION {
        DIR_LEFT = -1,
        DIR_RIGHT = 1,
        DIR_UP = 2,
        DIR_DOWN= -2,
        DIR_MAX
    };
    struct POINT {
        int x = 0;
        int z = 0;

        void Set(int x, int z) {
            this->x = x;
            this->z = z;
        }
        bool operator == (const POINT pts) const {
            return (x == pts.x && z == pts.z);
        }
        POINT operator = (const POINT pts) const {
            return pts;
        }
        POINT operator + (const POINT& pts) {
            POINT ret;
            ret.x = x + pts.x;
            ret.z = z + pts.z;
            return ret;
        }
        POINT operator - (const POINT& pts) {
            POINT ret;
            ret.x = x - pts.x;
            ret.z = z - pts.z;
            return ret;
        }
        POINT operator += (const POINT& pts) {
            return (*this + pts);
        }
        POINT operator -= (const POINT& pts) {
            return (*this - pts);
        }

    };


    struct FILLPOINT : POINT
    {
        std::vector<DIRECTION> prevDir;
    };

    void Fill(int _x, int _z, BLOCKTYPE _type);
    void StoreDirToPoint(POINT &pts, DIRECTION dir);


public:
    //コンストラクタ
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Set();
    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);

    //偽プロシージャ
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};