#pragma once
#include"CXCharacter.h"
#include "CInput.h"
#include"CCollider.h"

class CXPlayer :public CXCharacter
{
public:
	
	//�X�V����
	void Update();
private:
	//�R���C�_�̐錾
	CCollider mColSphereBody;	//��
	CCollider mColSphereHead;	//��
	CCollider mColSphereSword;	//��

	CInput mInput;

};