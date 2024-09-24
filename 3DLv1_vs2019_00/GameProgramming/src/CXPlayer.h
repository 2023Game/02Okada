#pragma once
#include"CXCharacter.h"
#include "CInput.h"
#include"CCollider.h"

class CXPlayer :public CXCharacter
{
public:
	
	//更新処理
	void Update();
private:
	//コライダの宣言
	CCollider mColSphereBody;	//体
	CCollider mColSphereHead;	//頭
	CCollider mColSphereSword;	//剣

	CInput mInput;

};