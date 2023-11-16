#include "CBullet.h"


#define VELOCITY CVector(0.0f, 0.0f, 1.0f) //移動速度

//幅と奥行きの設定
//Set(幅, 奥行)
void CBullet::Set(float w, float d) 
{
	//スケール設定
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//三角形の頂点設定→ヒント 3
	

		//三角形の法線設定
		mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//更新
void CBullet::Update() 
{
	CTransform::Update();
	//位置更新　進行方向へ１進む→ヒント 17
	mPosition = mPosition + VELOCITY * mMatrixRotate;
}

//描画
void CBullet::Render() 
{
	//DIFFUSE黄色設定
	float c[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//三角形描画→ヒント 12
		CVector mV[3], mN[3];
		mV[0] = CTriangle::mV[0] * m;
		mV[1] = CTriangle::mV[1] * m;
		mV[2] = CTriangle::mV[2] * m;
		mN[0] = CTriangle::mN[0] * m;
		mN[1] = CTriangle::mN[1] * m;
		mN[2] = CTriangle::mN[2] * m;
		glBegin(GL_TRIANGLES);
		glNormal3f(mN[0].X(), mN[0].Y(), mN[0].Z());
		glTexCoord2f(mUv[0].X(), mUv[0].Y());
		glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());
		glNormal3f(mN[1].X(), mN[1].Y(), mN[1].Z());
		glTexCoord2f(mUv[1].X(), mUv[1].Y());
		glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());
		glNormal3f(mN[2].X(), mN[2].Y(), mN[2].Z());
		glTexCoord2f(mUv[2].X(), mUv[2].Y());
		glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());
		glEnd();
}
