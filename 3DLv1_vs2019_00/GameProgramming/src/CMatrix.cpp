#include "CMatrix.h"
#include"CTriangle.h"
#include"CModel.h"
#include"CVector.h"
//標準入出力関数のインクルード
#include <stdio.h>

void CMatrix::Print() 
{
	printf("%10f %10f %10f %10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f %10f %10f %10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f %10f %10f %10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f %10f %10f %10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}

//デフォルトコンストラクタ
CMatrix::CMatrix() 
{
	Identity();
}
//単位行列の作成
CMatrix CMatrix::Identity() 
{
	//1行目を設定
	mM[0][0] = 1.0f; mM[0][1] = 0.0f; mM[0][2] = 0.0f; mM[0][3] = 0.0f;
	//2行目を設定
	mM[1][0] = 0.0f; mM[1][1] = 1.0f; mM[1][2] = 0.0f; mM[1][3] = 0.0f;
	//3行目を設定
	mM[2][0] = 0.0f; mM[2][1] = 0.0f; mM[2][2] = 1.0f; mM[2][3] = 0.0f;
	//4行目を設定
	mM[3][0] = 0.0f; mM[3][1] = 0.0f; mM[3][2] = 0.0f; mM[3][3] = 1.0f;

	//この行列を返す
	return *this;
}

float CMatrix::M(int r, int c) const
{
	return mM[r][c];
}

// 拡大縮小行列の作成
//Scale(倍率X, 倍率Y, 倍率Z)
CMatrix CMatrix::Scale(float sx, float sy, float sz) 
{
	//1行目を設定
	mM[0][0] = sx; mM[0][1] = 0.0f; mM[0][2] = 0.0f; mM[0][3] = 0.0f;
	//2行目を設定
	mM[1][0] = 0.0f; mM[1][1] = sx; mM[1][2] = 0.0f; mM[1][3] = 0.0f;
	//3行目を設定
	mM[2][0] = 0.0f; mM[2][1] = 0.0f; mM[2][2] = sx; mM[2][3] = 0.0f;
	//4行目を設定
	mM[3][0] = 0.0f; mM[3][1] = 0.0f; mM[3][2] = 0.0f; mM[3][3] = 1.0f;

	//この行列を返す
	return *this;
}

