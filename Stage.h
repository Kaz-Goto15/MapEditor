#include "Engine/GameObject.h"
#include "resource.h"
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

public:
    //コンストラクタ
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);

    //偽プロシージャ
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};