#include "Engine/GameObject.h"
#include <dinput.h>

//ステージを管理するクラス
class Controller : public GameObject
{
    enum KEY {
        MV_FRONT = 0,
        MV_BACK,
        MV_LEFT,
        MV_RIGHT,
        MV_UP ,
        MV_DOWN,
        MV_MAX
    };
    int movcode[MV_MAX]{ 0 };
    float movSpdRotate_[MV_MAX]{ 0 };
    float movSpd_, movMaxSpd_;
    float mouseMovReg_, keyMovReg_;
public:
    //コンストラクタ
    Controller(GameObject* parent);

    //デストラクタ
    ~Controller();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};