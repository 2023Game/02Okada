#include "CMatrix.h"
#include"CTriangle.h"
#include"CModel.h"
#include"CVector.h"
//�W�����o�͊֐��̃C���N���[�h
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

//�f�t�H���g�R���X�g���N�^
CMatrix::CMatrix() 
{
	Identity();
}
//�P�ʍs��̍쐬
CMatrix CMatrix::Identity() 
{
	//1�s�ڂ�ݒ�
	mM[0][0] = 1.0f; mM[0][1] = 0.0f; mM[0][2] = 0.0f; mM[0][3] = 0.0f;
	//2�s�ڂ�ݒ�
	mM[1][0] = 0.0f; mM[1][1] = 1.0f; mM[1][2] = 0.0f; mM[1][3] = 0.0f;
	//3�s�ڂ�ݒ�
	mM[2][0] = 0.0f; mM[2][1] = 0.0f; mM[2][2] = 1.0f; mM[2][3] = 0.0f;
	//4�s�ڂ�ݒ�
	mM[3][0] = 0.0f; mM[3][1] = 0.0f; mM[3][2] = 0.0f; mM[3][3] = 1.0f;

	//���̍s���Ԃ�
	return *this;
}

float CMatrix::M(int r, int c) const
{
	return mM[r][c];
}

// �g��k���s��̍쐬
//Scale(�{��X, �{��Y, �{��Z)
CMatrix CMatrix::Scale(float sx, float sy, float sz) 
{
	//1�s�ڂ�ݒ�
	mM[0][0] = sx; mM[0][1] = 0.0f; mM[0][2] = 0.0f; mM[0][3] = 0.0f;
	//2�s�ڂ�ݒ�
	mM[1][0] = 0.0f; mM[1][1] = sy; mM[1][2] = 0.0f; mM[1][3] = 0.0f;
	//3�s�ڂ�ݒ�
	mM[2][0] = 0.0f; mM[2][1] = 0.0f; mM[2][2] = sz; mM[2][3] = 0.0f;
	//4�s�ڂ�ݒ�
	mM[3][0] = 0.0f; mM[3][1] = 0.0f; mM[3][2] = 0.0f; mM[3][3] = 1.0f;

	//���̍s���Ԃ�
	return *this;
}
