#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Camera.h"



#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")
class Texture;

class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;	//matWからNormalように改名
		XMFLOAT4 diffuseColor;	//色
		int isTexture;			//テクスチャが貼っているか
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	int vertexCount_;	//頂点数
	int indexNum_;//インデックス数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;		//頂点バッファ
	ID3D11Buffer** pIndexBuffer_;		//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;		//コンスタントバッファ
	MATERIAL* pMaterialList_;

public:

	Fbx();
	HRESULT Load(std::string fileName);
	void InitVertex(fbxsdk::FbxMesh* mesh);//頂点バッファ準備
	void InitIndex(fbxsdk::FbxMesh* mesh);//インデックスバッファ準備
	void IntConstantBuffer();	//コンスタントバッファ準備
	void InitMaterial(fbxsdk::FbxNode* pNode);

	void Draw(Transform& transform);

	//	void PassDataToCB(XMMATRIX worldMatrix);
		//各バッファをパイプラインにセット
	void SetBufferToPipeline();
	void Release();
};