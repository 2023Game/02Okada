#include"CMaterial.h"
//vectorのインクルード
#include <vector>
#include"CTriangle.h"
#include"CVertex.h"
#ifndef CMODEL_H
#define CMODEL_H
/*
モデルクラス
モデルデータの入力や表示
*/
class CModel {
public:
	const std::vector<CTriangle>& Triangles() const;
	//描画
	//Render(行列)
	void Render(const CMatrix& m);
	~CModel();
	//モデルファイルの入力
	//Load(モデルファイル名, マテリアルファイル名)
	void Load(char* obj, char* mtl);
	//描画
	void Render();

private:
	//頂点の配列
	CVertex* mpVertexes;
	void CreateVertexBuffer();
	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
};

#endif

