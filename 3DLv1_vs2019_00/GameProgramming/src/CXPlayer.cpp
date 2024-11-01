#include"CXPlayer.h"
#include"CXCharacter.h"
#include"CCollider.h"




void CXPlayer::Update()
{
    switch (AnimationIndex())
    {
    case 3:
        if (IsAnimationFinished())
        {
            ChangeAnimation(4, false, 30);
        }
        break;
    case 4:
        if (IsAnimationFinished())
        {
            ChangeAnimation(0, true, 60);
        }
        break;
    default:
        if (mInput.Key('A')) 
        {
            mRotation.Y(mRotation.Y() + 2.0f);
        }
        if (mInput.Key('D')) 
        {
            mRotation.Y(mRotation.Y() - 2.0f);
        }
        if (mInput.Key('W')) 
        {
            ChangeAnimation(1, true, 60);
            mPosition = mPosition + MatrixRotate().VectorZ() * 0.1f;
        }
        else
        {
            ChangeAnimation(0, true, 60);
        }
        if (mInput.Key(' '))
        {
            ChangeAnimation(3, false, 30);
        }
    }
    CXCharacter::Update();
}

//コライダの初期化
CXPlayer();
    : mColSphereBody(this, nullptr, CVector(), 0.5f)
    , mColSphereHead(this, nullptr,
        CVector(0.0f, 5.0f, -3.0f), 0.5f)
    , mColSphereSword(this, nullptr,
        CVector(-10.0f, 10.0f, 50.0f), 0.3f)

{
}


void CXPlayer::Init(CModelX* model)
{
    CXCharacter::Init(model);
    //合成行列の設定
    mColSphereBody.Matrix(&mpCombinedMatrix[8]);
    //頭
    mColSphereHead.Matrix(&mpCombinedMatrix[11]);
    //剣
    mColSphereSword.Matrix(&mpCombinedMatrix[21]);

}

void CCollider::Matrix(CMatrix* m)
{
    mpMatrix = m;
}


