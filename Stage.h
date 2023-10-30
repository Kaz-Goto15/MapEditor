#include "Engine/GameObject.h"
#include "resource.h"
#include <vector>
#include <iostream>
#include <bitset>

using std::bitset;

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
//�X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{
	int hModel_[MODEL_NUM];    //���f���ԍ�
	struct {
		BLOCKTYPE bType;
		int height;
	}table_[Z_SIZE][X_SIZE];

	int mode_;      //0:������ 1:������ 2:��ރJ�G�� 3:�ʂ�Ԃ�
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

	enum DIRECTION {
		DIR_LEFT = 0b1000,
		DIR_RIGHT = 0b0100,
		DIR_UP = 0b0010,
		DIR_DOWN = 0b0001,
		DIR_MAX = sizeof(DIRECTION),
	};
	//DIRECTION begin(DIRECTION) { return DIRECTION::DIR_LEFT; }
//DIRECTION end(DIRECTION) { return DIRECTION::DIR_MAX; }
//DIRECTION operator*(DIRECTION dir) { return dir; }
//DIRECTION operator++(DIRECTION& dir) {
//	return dir = DIRECTION(std::underlying_type<DIRECTION>::type(dir) + 1);
//}
//std::ostream& operator<<(std::ostream& os, DIRECTION dir) {
//	switch (dir) {
//	case DIRECTION::DIR_LEFT: return os << "Top";
//	}
//}
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
		bitset<DIR_MAX> dirBit = 0;
	};

	void Fill(int _x, int _z, BLOCKTYPE _type);
	void StoreDirToPoint(POINT& pts, DIRECTION dir);
	POINT StoreDirToPoint(DIRECTION dir);
	DIRECTION ReverseDir(DIRECTION dir);

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
	void SetBlock(POINT pts, BLOCKTYPE _type);
	void SetBlockHeight(int _x, int _z, int _height);

	//�U�v���V�[�W��
	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};