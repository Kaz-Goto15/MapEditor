#include "Engine/GameObject.h"

//ステージを管理するクラス
class Controller : public GameObject
{
    XMFLOAT3 CamPosition_, CamTarget_;
    float movUnit_;
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