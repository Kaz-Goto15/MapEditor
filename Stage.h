#include "Engine/GameObject.h"
#include "resource.h"
#include <vector>
#include <iostream>
#include <bitset>
#include <sstream>
using std::bitset;
using std::vector;

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
		CHANGE = IDC_RADIO_CHANGE,
		FILL = IDC_RADIO_FILL,
	};
}
namespace NewProjSetUp {
	
	void Init(HWND hDlg);
}

//ステージを管理するクラス
class Stage : public GameObject
{
public:
	Stage(GameObject* parent);
	~Stage();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	/// <summary>
	/// 呼び出すことで現在の設定でステージのモディファイを行う
	/// つまりはクリック時の動作
	/// </summary>
	void Act();

	//void SetBlock(int _x, int _z, BLOCKTYPE _type);
	//void SetBlock(POINT pts, BLOCKTYPE _type);
	//偽プロシージャ
	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void NPSUIni(HWND hDlg);
private:
	int hModel_[MODEL_NUM];    //モデル番号
	struct MAP_TABLE {
		BLOCKTYPE bType;
		int height;
	}table_[Z_SIZE][X_SIZE];
	//vector<vector<MAP_TABLE>> table_(Z_SIZE, vector<MAP_TABLE>(X_SIZE), DEFAULT);
	//std::vector<std::vector<MAP_TABLE>> table_();
	//int n = 3;
	//vector < vector<int> > vv(n);
	int mode_;      //0:あげる 1:さげる 2:種類カエル 3:ぬりつぶし
	int select_;    //種類
	bool isEdited_;

	enum DIRECTION {
		DIR_LEFT = 0,
		DIR_RIGHT,
		DIR_UP,
		DIR_DOWN,
		DIR_MAX = sizeof(DIRECTION),
	};
	vector<bitset<DIR_MAX>> dirBit = { 0b1000,0b0100,0b0010,0b0001 };

	typedef struct POINT {
		int x = 0;
		int z = 0;

		void Set(int x, int z) {
			this->x = x;
			this->z = z;
		}
		void Set(POINT pts) {
			Set(pts.x, pts.z);
		}
		POINT GetPoint() { return *this; }
		bool operator == (const POINT pts) const {
			return (x == pts.x && z == pts.z);
		}
		void operator = (const POINT pts) {
			this->x = pts.x;
			this->z = pts.z;
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
		bitset<DIR_MAX> prevDirBit = 0;
	};

	void SetBlock(int _x, int _z, BLOCKTYPE _type);
	void SetBlock(POINT pts, BLOCKTYPE _type);
	void SetBlockHeight(int _x, int _z, int _height);
	void Fill(int _x, int _z, BLOCKTYPE _type);

	bitset<DIR_MAX> GetDirBit(DIRECTION dir) { return dirBit[dir]; }
	POINT StoreDirToPoint(DIRECTION dir);
	DIRECTION ReverseDir(DIRECTION dir);
	bool IsExistsWithin(POINT pts);
	//std::stringstream ss;
	//std::string dirStr[DIR_MAX] = { "左","右","上","下" };
	//std::string bTypeStr[sizeof(BLOCKTYPE)+1] = { "DEFAULT","BRICK","GRASS","SAND","WATER" };
	//std::string dirBitToStr(bitset<DIR_MAX> db);

	void NewFile();
	bool SaveFile();
	bool LoadFile();
	bool ConfirmDestruct();
	void LoadData(DWORD _fileSize, char* _puredata, vector<vector<std::string>>* _data);
	void GetSingleData(std::string* result, std::string data, DWORD* index);

	//void NPSUIni(HWND hDlg);
	const char* iniPath;
	void NewProjSetUp();
};