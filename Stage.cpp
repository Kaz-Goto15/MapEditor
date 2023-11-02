#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <string>
#include "Controller.h"

using std::string;

namespace NewProjSetUp {
	void Init(HWND hDlg) {
		int newW, newH;
		const char* iniPath = ".\\Assets/mapeditor.ini";
		::GetPrivateProfileInt(objectName_.c_str(), "new_w", newW, iniPath);
		::GetPrivateProfileInt(objectName_.c_str(), "new_h", newH, iniPath);
		// ダイアログの初期化処理
		SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_WIDTH, WM_SETTEXT, 0, (LPARAM)newW);
		SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_HEIGHT, WM_SETTEXT, 0, (LPARAM)newH);
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"レンガ");
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"草");
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"砂");
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"水");
	}
}

//コンストラクタ
Stage::Stage(GameObject* parent) :
	GameObject(parent, "Stage"),
	mode_(-1),
	select_(-1),
	isEdited_(false)
{
	std::fill(hModel_, hModel_ + MODEL_NUM, -1);
	//table_.resize(Z_SIZE, vector<MAP_TABLE>(X_SIZE));
	//NewFile();
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
			SetBlock(x, z, (BLOCKTYPE)((x + z) % MODEL_NUM));
		}
	}

}

//更新
void Stage::Update()
{
}

//描画
void Stage::Draw()
{
	Transform stageTrans;
	for (int z = 0; z < Z_SIZE; z++) {
		stageTrans.position_.z = (float)z;
		for (int x = 0; x < X_SIZE; x++) {
			stageTrans.position_.x = (float)x;
			for (int y = 0; y < table_[z][x].height + 1; y++) {
				stageTrans.position_.y = (float)y;
				//table_[z][x].
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
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"レンガ");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"草");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"砂");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"水");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_SETCURSEL, 0, 0);
		select_ = BLOCKTYPE::DEFAULT;
		return TRUE;
	case WM_COMMAND:
		if (SendMessage(GetDlgItem(hDlg, LOWORD(wp)), BM_GETCHECK, 0, 0) == BST_CHECKED) {
			mode_ = LOWORD(wp);
		}
		if (LOWORD(wp) == IDC_COMBO_BLOCKTYPE) {
			select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_GETCURSEL, 0, 0);
			break;
		}
		if (LOWORD(wp) == ID_MENU_SAVE) { SaveFile();	break; }
		if (LOWORD(wp) == ID_MENU_OPEN) { LoadFile();	break; }
		if (LOWORD(wp) == ID_MENU_NEW)	{ NewFile();	break; }
	}
	return FALSE;
}

void Stage::Act()
{
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
					trans.position_.x = (float)x;
					trans.position_.y = (float)y;
					trans.position_.z = (float)z;
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
			isEdited_ = true;
			switch (mode_) {
			case MODE::UP:
				SetBlockHeight(changeTile[0], changeTile[1], table_[changeTile[1]][changeTile[0]].height += 1);
				break;
			case MODE::DOWN:
				if (table_[changeTile[1]][changeTile[0]].height > 0)
					SetBlockHeight(changeTile[0], changeTile[1], table_[changeTile[1]][changeTile[0]].height -= 1);
				break;
			case MODE::CHANGE:
				SetBlock(changeTile[0], changeTile[1], (BLOCKTYPE)select_);
				break;
			case MODE::FILL:
				Fill(changeTile[0], changeTile[1], (BLOCKTYPE)select_);
				break;
			}
		}
}

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
	table_[_z][_x].bType = _type;
}

void Stage::SetBlock(POINT pts, BLOCKTYPE _type)
{
	SetBlock(pts.x, pts.z, _type);
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
	table_[_z][_x].height = _height;
}


void Stage::Fill(int _x, int _z, BLOCKTYPE _type)
{
	//ss.str(""); ss << "Trigger:[" << _z << "," << _x << "]\n";OutputDebugString(ss.str().c_str());

	//まず自身の種類を記憶し、塗る
	BLOCKTYPE fillType = table_[_z][_x].bType;
	SetBlock(_x, _z, _type);
	//ss.str(""); ss << fillType << bTypeStr[fillType] << " > " << _type << bTypeStr[_type] << "\n"; OutputDebugString(ss.str().c_str());
	
	//自身を塗りつぶしリストに追加
	vector<FILLPOINT> fillList;
	FILLPOINT tgtBase;
	tgtBase.Set(_x, _z);
	fillList.push_back(tgtBase);
	
	//塗りつぶしリストが空になるまでループ
	//リスト内には座標と塗られた方向が格納されている
	while (fillList.size() > 0) {
		FILLPOINT fTgtBase = fillList.front();
		fillList.erase(fillList.begin());

		//前方向の逆を調べる
		bitset<DIR_MAX> findDir = ~fTgtBase.prevDirBit;	//前方向の反転ビット
		//ss.str(""); ss << "Watch:[" << fTgtBase.z << "," << fTgtBase.x << "] Dir:" << dirBitToStr(findDir) << "\n"; OutputDebugString(ss.str().c_str());
		for (DIRECTION d = DIR_LEFT; d < DIR_MAX; d = static_cast<DIRECTION>(d + 1)) {
			bitset<DIR_MAX> dBit(dirBit[d]);
			if ((findDir & dBit) == dBit) {
				//調べる方向に1マス伸ばした座標を見る
				POINT tgt = fTgtBase.GetPoint() + StoreDirToPoint(d);
				//ss.str(""); ss << "Find:[" << tgt.z << "," << tgt.x << "] DirFromWatch: " << dirStr[d] << " Result: "; OutputDebugString(ss.str().c_str());
				
				//マップ外ならcontinue
				if (!IsExistsWithin(tgt)) {
					//ss.str(""); ss << "OutofRange\n"; OutputDebugString(ss.str().c_str());
					continue;
				};

				//既にリストにある場合、調べている方向の逆方向を追加
				bool isExists = false;
				for (auto& fL : fillList) {
					if (fL.GetPoint() == tgt.GetPoint()) {
						//ss.str(""); ss << "Already Added\n"; OutputDebugString(ss.str().c_str());
						isExists = true;
						fL.prevDirBit |= dirBit[ReverseDir(d)];
						break;
					}
				}
				//リストにない場合
				//ブロックが置換対象タイプであれば置き換え、リストにその座標と調べている方向の逆方向を追加
				if (!isExists) {
					if (table_[tgt.z][tgt.x].bType == fillType) {
						//ss.str(""); ss << "Fill&Add\n"; OutputDebugString(ss.str().c_str());
						SetBlock(tgt, _type);
						FILLPOINT pushPts;
						pushPts.Set(tgt);
						pushPts.prevDirBit |= dirBit[ReverseDir(d)];
						fillList.push_back(pushPts);
					}
					//else ss.str(""); ss << "Unmatch BlockType(" << bTypeStr[table_[tgt.z][tgt.x].bType] << ")\n"; OutputDebugString(ss.str().c_str());
				}
			}
		}
	}

}

Stage::POINT Stage::StoreDirToPoint(DIRECTION dir)
{
	switch (dir)
	{
	case Stage::DIR_LEFT:	return { -1, 0 };
	case Stage::DIR_RIGHT:	return { 1, 0 };
	case Stage::DIR_UP:		return { 0, 1 };
	case Stage::DIR_DOWN:	return { 0,-1 };
	}						return { 0,0 };
}

Stage::DIRECTION Stage::ReverseDir(DIRECTION dir)
{
	switch (dir) {
	case Stage::DIR_LEFT:	return Stage::DIR_RIGHT;
	case Stage::DIR_RIGHT:	return Stage::DIR_LEFT;
	case Stage::DIR_UP:		return Stage::DIR_DOWN;
	case Stage::DIR_DOWN:	return Stage::DIR_UP;
	}						return dir;
}

bool Stage::IsExistsWithin(POINT pts)
{
	if (pts.x < 0 || pts.z < 0 || pts.x >= X_SIZE || pts.z >= Z_SIZE)
		return false;
	return true;
}

//std::string Stage::dirBitToStr(bitset<DIR_MAX> db)
//{
//	std::string ret;
//	for (DIRECTION d = DIR_LEFT; d < DIR_MAX; d = static_cast<DIRECTION>(d + 1)) {
//		bitset<DIR_MAX> dBit(dirBit[d]);
//		if ((db & dBit) == dBit)ret.append(dirStr[d]);
//	}
//		return ret;
//}

void Stage::NewFile()
{
	//table_.resize(Z_SIZE, vector<MAP_TABLE>(X_SIZE));
	if (ConfirmDestruct()) {
		NewProjSetUp();

		for (int z = 0; z < Z_SIZE; z++) {
			for (int x = 0; x < X_SIZE; x++) {
				SetBlock(x, z, DEFAULT);
				SetBlockHeight(x, z, 0);
			}
		}
		isEdited_ = false;
	}
}

bool Stage::SaveFile()
{
	char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

	//「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")			//─┬ファイルの種類
		TEXT("すべてのファイル(*.*)\0*.*\0\0");                     //─┘
	ofn.lpstrFile = fileName;               	//ファイル名
	ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
	ofn.Flags = OFN_OVERWRITEPROMPT;   		//フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "map";                  	//デフォルト拡張子

	//「ファイルを保存」ダイアログ
	BOOL selFile;
	selFile = GetSaveFileName(&ofn);

	//キャンセルしたら中断
	if (selFile == FALSE) return false;

	HANDLE hFile;
	//ファイルを開く
	hFile = CreateFile(
		fileName,  //ファイル名
		GENERIC_WRITE,              //アクセスモード（書き込み用）
		0,                          //共有（なし）
		NULL,                       //セキュリティ属性（継承しない）
		CREATE_ALWAYS,              //作成方法
		FILE_ATTRIBUTE_NORMAL,      //属性とフラグ（設定なし）
		NULL);                      //拡張属性（なし）

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, ("CANNOT OPEN FILE: " + (string)fileName).c_str(), "Error", MB_OK);
		return false;
	}

	//データ書込
	std::string writeData;
	for (int z = 0; z < Z_SIZE; z++) {
		for (int x = 0; x < X_SIZE; x++) {
			std::stringstream ss;
			ss << z << "," << x << "," << table_[z][x].height << "," << table_[z][x].bType << "\r\n";
			writeData += ss.str();
		}
	}
	writeData += "\0";

	DWORD dwBytes = 0;				//書き込み位置
	BOOL res = WriteFile(
		hFile,								//ファイルハンドル
		writeData.c_str(),                  //保存するデータ（文字列）
		(DWORD)strlen(writeData.c_str()),   //書き込む文字数
		&dwBytes,							//書き込んだサイズを入れる変数
		NULL);								//オーバーラップド構造体（今回は使わない）

	if (res == FALSE) {
		MessageBox(NULL, ("FAILED TO WRITE: " + (string)fileName).c_str(), "Error", MB_OK);
		return false;
	}

	//ファイルを閉じる
	CloseHandle(hFile);

}

bool Stage::LoadFile()
{

	char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

	//「ファイルを開く」ダイアログの設定
	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")			//─┬ファイルの種類
		TEXT("すべてのファイル(*.*)\0*.*\0\0");                     //─┘
	ofn.lpstrFile = fileName;               	//ファイル名
	ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
	ofn.Flags = OFN_OVERWRITEPROMPT;   		//フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "map";                  	//デフォルト拡張子

	//「ファイルを開く」ダイアログ
	BOOL selFile;
	selFile = GetOpenFileName(&ofn);

	//キャンセルしたら中断
	if (selFile == FALSE) return false;

	if (!ConfirmDestruct())return false;
	HANDLE hFile;
	//ファイルを開く
	hFile = CreateFile(
		fileName,  //ファイル名
		GENERIC_READ,              //アクセスモード（書き込み用）
		0,                          //共有（なし）
		NULL,                       //セキュリティ属性（継承しない）
		OPEN_EXISTING,              //作成方法
		FILE_ATTRIBUTE_NORMAL,      //属性とフラグ（設定なし）
		NULL);                      //拡張属性（なし）

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, ("CANNOT OPEN FILE: " + (string)fileName).c_str(), "Error", MB_OK);
		return false;
	}

	//ファイルサイズ取得
	DWORD fileSize = GetFileSize(hFile, NULL);

	//ファイルのサイズ分メモリを確保
	char* data = new char[fileSize];

	DWORD dwBytes = 0; //読み込み位置
	BOOL res = ReadFile(
		hFile,     //ファイルハンドル
		data,      //データを入れる変数
		fileSize,  //読み込むサイズ
		&dwBytes,  //読み込んだサイズ
		NULL);     //オーバーラップド構造体（今回は使わない）

	if (res == FALSE) {
		MessageBox(NULL, ("FAILED TO READ: " + (string)fileName).c_str(), "Error", MB_OK);
		return false;
	}

	CloseHandle(hFile);

	//ここに持ってきたデータ読み込むコード
	//読込データ入れる配列
	std::vector<std::vector<std::string>> data_;

	LoadData(fileSize, data, &data_);

	//読み込んだデータを開放
	delete[] data;

	//データを現在のプロジェクトファイルに読込
	for (int l = 0; l < data_.size(); l++){
		table_[atoi(data_[l][0].c_str())][atoi(data_[l][1].c_str())].height = atoi(data_[l][2].c_str());
		table_[atoi(data_[l][0].c_str())][atoi(data_[l][1].c_str())].bType = (BLOCKTYPE)atoi(data_[l][3].c_str());
	}

	//全データ開放
	for (int y = 0; y < data_.size(); y++)
		for (int x = 0; x < data_[y].size(); x++)
			data_[y][x].clear();

	isEdited_ = false;
}

bool Stage::ConfirmDestruct()
{
	if (isEdited_) {
		int result = MessageBox(NULL, "現在の編集データは破棄されますがよろしいですか?", "Map Editor", MB_OKCANCEL | MB_ICONEXCLAMATION);
		if (result == IDOK)return true;
		return false;
	}
	return true;
}

void Stage::LoadData(DWORD _fileSize, char* _puredata, std::vector<std::vector<std::string>>* _data)
{
	//1行のデータ入れる配列
	std::vector<std::string> line;
	//調べる文字の位置
	DWORD index = 0;

	//最後の文字まで繰り返す
	while (index < _fileSize) {
		//index文字目から「,」か「改行」までの文字列を取得
		std::string val;
		GetSingleData(&val, _puredata, &index);

		//if 文字数が0 then 行末
		if (val.length() - 1 == 0) {
			_data->push_back(line);	//_dataに1行分追加
			line.clear();			//1行データをクリア
			continue;				//index++;
		}
		line.push_back(val);		//1行分のデータに追加
	}
}

void Stage::GetSingleData(std::string* result, std::string data, DWORD* index)
{
	//「,」まで一文字ずつresultに入れる
	while (data[*index] != ',' && data[*index] != '\n' && data[*index] != '\r')
	{
		*result += data[*index];
		(*index)++;
	}

	//最後に「\0」を付ける
	*result += '\0';
	(*index)++;
}

class MapTemplate {
public:
	virtual void Run() = 0;
protected:
	string templateName = "";
};
class MapTDefault : MapTemplate {
	MapTDefault(string name) { templateName = name; }
	void Run() override {
		//ブロックのタイプをセット
		for (int z = 0; z < Z_SIZE; z++) {
			for (int x = 0; x < X_SIZE; x++) {
				//Stage::SetBlock(x, z, (BLOCKTYPE)((x + z) % MODEL_NUM));
			}
		}
	}
};
void Stage::NPSUIni(HWND hDlg) {
	int newW, newH;
	const char* iniPath = ".\\Assets/mapeditor.ini";
	::GetPrivateProfileInt(objectName_.c_str(), "new_w", newW, iniPath);
	::GetPrivateProfileInt(objectName_.c_str(), "new_h", newH, iniPath);
	// ダイアログの初期化処理
	SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_WIDTH, WM_SETTEXT, 0, (LPARAM)newW);
	SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_HEIGHT, WM_SETTEXT, 0, (LPARAM)newH);
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"レンガ");
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"草");
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"砂");
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"水");
}

BOOL CALLBACK NewProjSetUpDialogProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		NewProjSetUp::Init(hDlg);
		return TRUE;
	case WM_COMMAND:
		// ダイアログのコマンド処理
		if (LOWORD(wParam) == IDOK) {
			SendDlgItemMessage(hDlg,IDC_NEWSETUP_EDIT_WIDTH,EM_GETLINE,0,(LPARAM)X_SIZE);
			SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_HEIGHT, EM_GETLINE, 0, (LPARAM)Z_SIZE);
		}
		if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, IDCANCEL);
		}
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, IDCANCEL);
		return TRUE;
	default:
		return FALSE;
	}
}

void Stage::NewProjSetUp()
{
	DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG_NEWSETUP), NULL, (DLGPROC)(NewProjSetUpDialogProc));
}