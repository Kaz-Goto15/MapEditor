#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Camera.h"
#include <vector>


#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

using std::vector;

class Texture;

struct RayCastData {
	XMFLOAT4 start;
	XMFLOAT4 dir;
	bool hit;
	float dist;

	RayCastData() { dist = 99999.0f; }
};

class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture*	pTexture;
		XMFLOAT4	diffuse;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;		//matWからNormalように改名
		XMFLOAT4	diffuseColor;	//色
		int			isTextured;		//テクスチャが貼っているか
	};

	struct VERTEX
	{
		XMVECTOR position;	//位置
		XMVECTOR uv;		//テクスチャ座標
		XMVECTOR normal;	//法線ベクトル
	};

	VERTEX* pVertices_;
	int** ppIndex_;

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	ID3D11Buffer*	pVertexBuffer_;		//頂点バッファ
	ID3D11Buffer**	pIndexBuffer_;		//インデックスバッファ
	ID3D11Buffer*	pConstantBuffer_;	//コンスタントバッファ
	MATERIAL*		pMaterialList_;
	vector<int>		indexCount_;		//インデックス数

	void InitVertex(fbxsdk::FbxMesh* mesh);//頂点バッファ準備
	void InitIndex(fbxsdk::FbxMesh* mesh);//インデックスバッファ準備
	void IntConstantBuffer();	//コンスタントバッファ準備
	void InitMaterial(fbxsdk::FbxNode* pNode);
	//各バッファをパイプラインにセット
	//void SetBufferToPipeline();
public:

	Fbx();
	HRESULT Load(std::string fileName);
	void	Draw(Transform& transform);
	void	Release();

	void RayCast(RayCastData& rayData);
};