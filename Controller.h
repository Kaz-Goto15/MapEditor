#include "Engine/GameObject.h"
#include <dinput.h>
#include "resource.h"
namespace {
    enum KEY {
        MV_FRONT = 0,
        MV_BACK,
        MV_LEFT,
        MV_RIGHT,
        MV_UP,
        MV_DOWN,
        MV_MAX
    };
}
//操作を管理するクラス
class Controller : public GameObject
{
    int movcode[MV_MAX]{ 0 };
    float movSpdRotate_[MV_MAX]{ 0 };
    float movSpd_, movMaxSpd_;
    float mouseMovReg_, keyMovAcc_, keyMovDec_ ;
    bool enMovInertia_;
    bool canMouseCtlRotate_;

public:
    Controller(GameObject* parent);
    ~Controller();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //偽プロシージャ
    //BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};