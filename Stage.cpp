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
		// �_�C�A���O�̏���������
		SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_WIDTH, WM_SETTEXT, 0, (LPARAM)newW);
		SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_HEIGHT, WM_SETTEXT, 0, (LPARAM)newH);
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"�����K");
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"��");
	}
}

//�R���X�g���N�^
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

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
	std::string modelName[] = {
		"BoxDefault","BoxBrick","BoxGrass","BoxSand", "BoxWater"
	};

	//���f���f�[�^�̃��[�h
	std::string ext = "fbx";
	std::string folder = "Assets/";
	for (int i = 0; i < MODEL_NUM; i++) {
		hModel_[i] = Model::Load(folder + modelName[i] + "." + ext);
		assert(hModel_[i] >= 0);
	}
	//�u���b�N�̃^�C�v���Z�b�g
	for (int z = 0; z < Z_SIZE; z++) {
		for (int x = 0; x < X_SIZE; x++) {
			SetBlock(x, z, (BLOCKTYPE)((x + z) % MODEL_NUM));
		}
	}

}

//�X�V
void Stage::Update()
{
}

//�`��
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

//�J��
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
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"�����K");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLOCKTYPE), CB_ADDSTRING, 0, (LPARAM)"��");
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
		//�r���[�|�[�g�s��
		float w = (float)(Direct3D::scrWidth / 2.0f);
		float h = (float)(Direct3D::scrHeight / 2.0f);
		//offsetx,y��0
		//minZ=0 maxZ=1
		XMMATRIX vp = {
			w,  0,  0,  0,
			0, -h,  0,  0,
			0,  0,  1,  0,
			w,  h,  0,  1
		};
		//�r���[�|�[�g
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		//�v���W�F�N�V�����ϊ�
		XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
		//�r���[�ϊ�
		XMMATRIX invview = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

		XMFLOAT3 mousePosFront = Input::GetMousePosition();
		mousePosFront.z = 0.0f;
		XMFLOAT3 mousePosBack = Input::GetMousePosition();
		mousePosBack.z = 1.0f;
		//mousePosFront XMVECTOR�ɕϊ�
		//�����invVP,invProj,invView��������
		//mousePosBack��XMVECTOR�ɕϊ�
		//����ɁV
		//mousePosFront����mousePosBack�ɂ����ă��C��ł� hModel_[0]
		//���C������������u���[�N�|�C���g�Ŏ~�߂�
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

	//�܂����g�̎�ނ��L�����A�h��
	BLOCKTYPE fillType = table_[_z][_x].bType;
	SetBlock(_x, _z, _type);
	//ss.str(""); ss << fillType << bTypeStr[fillType] << " > " << _type << bTypeStr[_type] << "\n"; OutputDebugString(ss.str().c_str());
	
	//���g��h��Ԃ����X�g�ɒǉ�
	vector<FILLPOINT> fillList;
	FILLPOINT tgtBase;
	tgtBase.Set(_x, _z);
	fillList.push_back(tgtBase);
	
	//�h��Ԃ����X�g����ɂȂ�܂Ń��[�v
	//���X�g���ɂ͍��W�Ɠh��ꂽ�������i�[����Ă���
	while (fillList.size() > 0) {
		FILLPOINT fTgtBase = fillList.front();
		fillList.erase(fillList.begin());

		//�O�����̋t�𒲂ׂ�
		bitset<DIR_MAX> findDir = ~fTgtBase.prevDirBit;	//�O�����̔��]�r�b�g
		//ss.str(""); ss << "Watch:[" << fTgtBase.z << "," << fTgtBase.x << "] Dir:" << dirBitToStr(findDir) << "\n"; OutputDebugString(ss.str().c_str());
		for (DIRECTION d = DIR_LEFT; d < DIR_MAX; d = static_cast<DIRECTION>(d + 1)) {
			bitset<DIR_MAX> dBit(dirBit[d]);
			if ((findDir & dBit) == dBit) {
				//���ׂ������1�}�X�L�΂������W������
				POINT tgt = fTgtBase.GetPoint() + StoreDirToPoint(d);
				//ss.str(""); ss << "Find:[" << tgt.z << "," << tgt.x << "] DirFromWatch: " << dirStr[d] << " Result: "; OutputDebugString(ss.str().c_str());
				
				//�}�b�v�O�Ȃ�continue
				if (!IsExistsWithin(tgt)) {
					//ss.str(""); ss << "OutofRange\n"; OutputDebugString(ss.str().c_str());
					continue;
				};

				//���Ƀ��X�g�ɂ���ꍇ�A���ׂĂ�������̋t������ǉ�
				bool isExists = false;
				for (auto& fL : fillList) {
					if (fL.GetPoint() == tgt.GetPoint()) {
						//ss.str(""); ss << "Already Added\n"; OutputDebugString(ss.str().c_str());
						isExists = true;
						fL.prevDirBit |= dirBit[ReverseDir(d)];
						break;
					}
				}
				//���X�g�ɂȂ��ꍇ
				//�u���b�N���u���Ώۃ^�C�v�ł���Βu�������A���X�g�ɂ��̍��W�ƒ��ׂĂ�������̋t������ǉ�
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
	char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

	//�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
	OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
	ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
	ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
	ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")			//�����t�@�C���̎��
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");                     //����
	ofn.lpstrFile = fileName;               	//�t�@�C����
	ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
	ofn.Flags = OFN_OVERWRITEPROMPT;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
	ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q

	//�u�t�@�C����ۑ��v�_�C�A���O
	BOOL selFile;
	selFile = GetSaveFileName(&ofn);

	//�L�����Z�������璆�f
	if (selFile == FALSE) return false;

	HANDLE hFile;
	//�t�@�C�����J��
	hFile = CreateFile(
		fileName,  //�t�@�C����
		GENERIC_WRITE,              //�A�N�Z�X���[�h�i�������ݗp�j
		0,                          //���L�i�Ȃ��j
		NULL,                       //�Z�L�����e�B�����i�p�����Ȃ��j
		CREATE_ALWAYS,              //�쐬���@
		FILE_ATTRIBUTE_NORMAL,      //�����ƃt���O�i�ݒ�Ȃ��j
		NULL);                      //�g�������i�Ȃ��j

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, ("CANNOT OPEN FILE: " + (string)fileName).c_str(), "Error", MB_OK);
		return false;
	}

	//�f�[�^����
	std::string writeData;
	for (int z = 0; z < Z_SIZE; z++) {
		for (int x = 0; x < X_SIZE; x++) {
			std::stringstream ss;
			ss << z << "," << x << "," << table_[z][x].height << "," << table_[z][x].bType << "\r\n";
			writeData += ss.str();
		}
	}
	writeData += "\0";

	DWORD dwBytes = 0;				//�������݈ʒu
	BOOL res = WriteFile(
		hFile,								//�t�@�C���n���h��
		writeData.c_str(),                  //�ۑ�����f�[�^�i������j
		(DWORD)strlen(writeData.c_str()),   //�������ޕ�����
		&dwBytes,							//�������񂾃T�C�Y������ϐ�
		NULL);								//�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

	if (res == FALSE) {
		MessageBox(NULL, ("FAILED TO WRITE: " + (string)fileName).c_str(), "Error", MB_OK);
		return false;
	}

	//�t�@�C�������
	CloseHandle(hFile);

}

bool Stage::LoadFile()
{

	char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

	//�u�t�@�C�����J���v�_�C�A���O�̐ݒ�
	OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
	ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
	ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
	ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")			//�����t�@�C���̎��
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");                     //����
	ofn.lpstrFile = fileName;               	//�t�@�C����
	ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
	ofn.Flags = OFN_OVERWRITEPROMPT;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
	ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q

	//�u�t�@�C�����J���v�_�C�A���O
	BOOL selFile;
	selFile = GetOpenFileName(&ofn);

	//�L�����Z�������璆�f
	if (selFile == FALSE) return false;

	if (!ConfirmDestruct())return false;
	HANDLE hFile;
	//�t�@�C�����J��
	hFile = CreateFile(
		fileName,  //�t�@�C����
		GENERIC_READ,              //�A�N�Z�X���[�h�i�������ݗp�j
		0,                          //���L�i�Ȃ��j
		NULL,                       //�Z�L�����e�B�����i�p�����Ȃ��j
		OPEN_EXISTING,              //�쐬���@
		FILE_ATTRIBUTE_NORMAL,      //�����ƃt���O�i�ݒ�Ȃ��j
		NULL);                      //�g�������i�Ȃ��j

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, ("CANNOT OPEN FILE: " + (string)fileName).c_str(), "Error", MB_OK);
		return false;
	}

	//�t�@�C���T�C�Y�擾
	DWORD fileSize = GetFileSize(hFile, NULL);

	//�t�@�C���̃T�C�Y�����������m��
	char* data = new char[fileSize];

	DWORD dwBytes = 0; //�ǂݍ��݈ʒu
	BOOL res = ReadFile(
		hFile,     //�t�@�C���n���h��
		data,      //�f�[�^������ϐ�
		fileSize,  //�ǂݍ��ރT�C�Y
		&dwBytes,  //�ǂݍ��񂾃T�C�Y
		NULL);     //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

	if (res == FALSE) {
		MessageBox(NULL, ("FAILED TO READ: " + (string)fileName).c_str(), "Error", MB_OK);
		return false;
	}

	CloseHandle(hFile);

	//�����Ɏ����Ă����f�[�^�ǂݍ��ރR�[�h
	//�Ǎ��f�[�^�����z��
	std::vector<std::vector<std::string>> data_;

	LoadData(fileSize, data, &data_);

	//�ǂݍ��񂾃f�[�^���J��
	delete[] data;

	//�f�[�^�����݂̃v���W�F�N�g�t�@�C���ɓǍ�
	for (int l = 0; l < data_.size(); l++){
		table_[atoi(data_[l][0].c_str())][atoi(data_[l][1].c_str())].height = atoi(data_[l][2].c_str());
		table_[atoi(data_[l][0].c_str())][atoi(data_[l][1].c_str())].bType = (BLOCKTYPE)atoi(data_[l][3].c_str());
	}

	//�S�f�[�^�J��
	for (int y = 0; y < data_.size(); y++)
		for (int x = 0; x < data_[y].size(); x++)
			data_[y][x].clear();

	isEdited_ = false;
}

bool Stage::ConfirmDestruct()
{
	if (isEdited_) {
		int result = MessageBox(NULL, "���݂̕ҏW�f�[�^�͔j������܂�����낵���ł���?", "Map Editor", MB_OKCANCEL | MB_ICONEXCLAMATION);
		if (result == IDOK)return true;
		return false;
	}
	return true;
}

void Stage::LoadData(DWORD _fileSize, char* _puredata, std::vector<std::vector<std::string>>* _data)
{
	//1�s�̃f�[�^�����z��
	std::vector<std::string> line;
	//���ׂ镶���̈ʒu
	DWORD index = 0;

	//�Ō�̕����܂ŌJ��Ԃ�
	while (index < _fileSize) {
		//index�����ڂ���u,�v���u���s�v�܂ł̕�������擾
		std::string val;
		GetSingleData(&val, _puredata, &index);

		//if ��������0 then �s��
		if (val.length() - 1 == 0) {
			_data->push_back(line);	//_data��1�s���ǉ�
			line.clear();			//1�s�f�[�^���N���A
			continue;				//index++;
		}
		line.push_back(val);		//1�s���̃f�[�^�ɒǉ�
	}
}

void Stage::GetSingleData(std::string* result, std::string data, DWORD* index)
{
	//�u,�v�܂ňꕶ������result�ɓ����
	while (data[*index] != ',' && data[*index] != '\n' && data[*index] != '\r')
	{
		*result += data[*index];
		(*index)++;
	}

	//�Ō�Ɂu\0�v��t����
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
		//�u���b�N�̃^�C�v���Z�b�g
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
	// �_�C�A���O�̏���������
	SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_WIDTH, WM_SETTEXT, 0, (LPARAM)newW);
	SendDlgItemMessage(hDlg, IDC_NEWSETUP_EDIT_HEIGHT, WM_SETTEXT, 0, (LPARAM)newH);
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"�����K");
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"��");
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"��");
	SendMessage(GetDlgItem(hDlg, IDC_NEWSETUP_COMBO_TEMPLATE), CB_ADDSTRING, 0, (LPARAM)"��");
}

BOOL CALLBACK NewProjSetUpDialogProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		NewProjSetUp::Init(hDlg);
		return TRUE;
	case WM_COMMAND:
		// �_�C�A���O�̃R�}���h����
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