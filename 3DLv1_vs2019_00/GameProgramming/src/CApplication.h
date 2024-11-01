#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CMiss.h"
#include <vector>
#include "CCharacterManager.h"
#include"CVector.h"
#include"CModel.h"
#include"CCharacter3.h"
#include "CGame.h"
#include "CSound.h"
#include"CTaskManager.h"
#include"CColliderTriangle.h"
#include"CColliderMesh.h"
#include"CModelX.h"
#include"CXCharacter.h"
#include"CXPlayer.h"
#include"CXEnemy.h"



class CApplication
{
private:
	//敵のインスタンス
	CXEnemy mXEnemy;
	//キャラクタのインスタンス
	CXPlayer mXPlayer;
	CMatrix mMatrix;
	CModelX mModelX;
	static CUi* spUi;	//UIクラスのポインタ
	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	//削除CColliderTriangle mColliderTriangle2;
	//三角コライダの作成
    //削除CColliderTriangle mColliderTriangle;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	//C5モデル
	CModel mModelC5;
	CPlayer mPlayer;
	CModel mBackGround; //背景モデル
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	CGame* mpGame;
	static CCharacterManager mCharacterManager;
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
//	CCharacter mRectangle;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
//	CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	CMiss* mpMiss;
	CVector mEye;
	//モデルクラスのインスタンス作成
	CModel mModel;
	//CCharacterのポインタの可変長配列
//	std::vector<CCharacter*> mCharacters;
public:
	~CApplication();
	static CUi* Ui();	//UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};