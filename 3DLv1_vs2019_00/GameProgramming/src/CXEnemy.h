#pragma once
#include "CXCharacter.h"
#include"CCollider.h"

class CXEnemy :public CXCharacter
{
public:
    void Init(CModelX* model);
    CXEnemy();
private:
    //コライダの宣言
    CCollider mColSphereBody;    //体
    CCollider mColSphereHead;    //頭
    CCollider mColSphereSword;    //剣
};
