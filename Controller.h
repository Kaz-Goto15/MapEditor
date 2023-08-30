#include "Engine/GameObject.h"
#include <dinput.h>

//ステージを管理するクラス
class Controller : public GameObject
{
    enum KEY {
        MV_FRONT = DIK_W,
        MV_BACK = DIK_S,
        MV_LEFT = DIK_A,
        MV_RIGHT = DIK_D,
        MV_UP = DIK_Q,
        MV_DOWN = DIK_E,
        MV_MAX
    };
    int movcode[MV_MAX];
    float movSpd_, movMaxSpd_;
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