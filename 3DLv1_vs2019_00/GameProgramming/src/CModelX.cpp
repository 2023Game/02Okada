#include <stdio.h>
#include<string.h>
#include"CModelX.h"
#include"glut.h"
#include <ctype.h>	//isspace関数の宣言
#include"CMatrix.h"
#include"CVector.h"
#include "CMaterial.h"

CModelX::CModelX()
    :mpPointer(nullptr)
{
    //mTokenを初期化
    memset(mToken, 0, sizeof(mToken));
}

void CModelX::Load(char* file)
{
    //
    //ファイルサイズを取得する
    //
    FILE* fp;        //ファイルポインタ変数の作成
    fp = fopen(file, "rb");//ファイルをオープンする
    if (fp == NULL) 
    {   
        //エラーチェック　　
        //printf("fopen error:%s\n", file);
        return;
    }
   

    //ファイルの最後へ移動
    fseek(fp, 0L, SEEK_END);
    //ファイルサイズの取得
    int size = ftell(fp);
    //ファイルサイズ+1バイト分の領域を確保
    char* buf = mpPointer = new char[size + 1];
    //
    //ファイルから3Dモデルデータを読み込む
    //
    //ファイルの先頭へ移動
    fseek(fp, 0L, SEEK_SET);
    //確保した領域にファイルサイズ分データを読み込む
    fread(buf, size, 1, fp);
    //最後に\0を設定する(文字列の終端)
    buf[size] = '\0';
    fclose(fp);    //ファイルをクローズする

     //文字列の最後まで繰り返し
    while (*mpPointer != '\0') 
    {
        GetToken();	//単語の取得
        //単語がFrameの場合
        //template 読み飛ばし
        if (strcmp(mToken, "template") == 0) {
            SkipNode();
        }
        //Material の時
        else if (strcmp(mToken, "Material") == 0) {
            new CMaterial(this);
        }

        else if (strcmp(mToken, "Frame") == 0) 
        {
            //フレームを作成する
            new CModelXFrame(this);
        }
        //単語がAnimationSetの場合
        else if (strcmp(mToken, "AnimationSet") == 0) 
        {
            new CAnimationSet(this);
        }

    }

    
    SAFE_DELETE_ARRAY(buf);   //確保した領域を解放する
    //スキンウェイトのフレーム番号設定
    SetSkinWeightFrameIndex();


}

/*
GetToken
文字列データから、単語を1つ取得する
*/
char* CModelX::GetToken() 
{
    char* p = mpPointer;
    char* q = mToken;
    //タブ(\t)空白( )改行(\r)(\n)，；”の区切り文字以外になるまで読み飛ばす
    while (*p != '\0' && IsDelimiter(*p)) p++;
    if (*p == '{' || *p == '}') 
    {
        //{または}ならmTokenに代入し次の文字へ
        *q++ = *p++;
    }
    else 
    {
        //タブ(\t)空白( )改行(\r)(\n)，；”の区切り文字、
        //または、}の文字になるまでmTokenに代入する
        while (*p != '\0' && !IsDelimiter(*p) && *p != '}')
            *q++ = *p++;
    }

    *q = '\0';	//mTokenの最後に\0を代入
    mpPointer = p;	//次の読み込むポイントを更新する

    //もしmTokenが//の場合は、コメントなので改行まで読み飛ばす
    /*
    strcmp(文字列1, 文字列2)
    文字列1と文字列2が等しい場合、0を返します。
    文字列1と文字列2が等しくない場合、0以外を返します。
    */
    if (!strcmp("//", mToken)) 
    {
        //改行まで読み飛ばす
        while (*p != '\0' && !strchr("\r\n", *p)) p++;
        //読み込み位置の更新
        mpPointer = p;
        //単語を取得する（再帰呼び出し）
        return GetToken();
    }
    return mToken;
}


/*
* IsDelimiter(c)
* cが\t \r \n スペースなどの空白文字
* または,;"などの文字であれば
* 区切り文字としてtrueを返す
*/
bool CModelX::IsDelimiter(char c)
{
    //cが0より小さいとき、falseを返す
    if (c < 0)
        return false;
    //isspace(c)
    //cが空白文字なら0以外を返す
    if (isspace(c) != 0)
        return true;
    /*
    strchr(文字列, 文字)
    文字列に文字が含まれていれば、
    見つかった文字へのポインタを返す
    見つからなかったらNULLを返す
    */
    if (strchr(",;\"", c) != NULL)
        return true;
    //区切り文字ではない
    return false;
}

CModelXFrame::~CModelXFrame()
{
    //子フレームを全て解放する
    std::vector<CModelXFrame*>::iterator itr;
    for (itr = mChild.begin(); itr != mChild.end(); itr++) 
    {
        delete* itr;
    }
    //名前のエリアを解放する
    SAFE_DELETE_ARRAY(mpName);

    if (mpMesh != nullptr)
    {
        delete mpMesh;
    }
}

CModelX::~CModelX()
{
    if (mFrame.size() > 0)
    {
        delete mFrame[0];
    }
    for (size_t i = 0; i < mAnimationSet.size(); i++)
    {
        delete mAnimationSet[i];
    }
    //マテリアルの解放
    for (size_t i = 0; i < mMaterial.size(); i++) 
    {
        delete mMaterial[i];
    }


}

/*
SkipNode
ノードを読み飛ばす
*/
void CModelX::SkipNode() 
{
    //文字が終わったら終了
    while (*mpPointer != '\0') 
    {
        GetToken();	//次の単語取得
        //{が見つかったらループ終了
        if (strchr(mToken, '{')) break;
    }
    int count = 1;
    //文字が終わるか、カウントが0になったら終了
    while (*mpPointer != '\0' && count > 0) 
    {
        GetToken();	//次の単語取得
        //{を見つけるとカウントアップ
        if (strchr(mToken, '{')) count++;
        //}を見つけるとカウントダウン
        else if (strchr(mToken, '}')) count--;
    }
}


/*
 CModelXFrame
 model：CModelXインスタンスへのポインタ
 フレームを作成する
 読み込み中にFrameが見つかれば、フレームを作成し、
 子フレームに追加する
*/
CModelXFrame::CModelXFrame(CModelX* model)
    : mpName(nullptr)
    , mIndex(0)
    ,mpMesh(nullptr)
{
    //現在のフレーム配列の要素数を取得し設定する
    mIndex = model->mFrame.size();
    //CModelXのフレーム配列に追加する
    model->mFrame.push_back(this);
    //変換行列を単位行列にする
    mTransformMatrix.Identity();
    //次の単語（フレーム名の予定）を取得する
    model->GetToken(); // frame name
    //フレーム名分エリアを確保する
    mpName = new char[strlen(model->mToken) + 1];
    //フレーム名をコピーする
    strcpy(mpName, model->mToken);
    //次の単語（{の予定）を取得する
    model->GetToken();  // 
    {
        //文字が無くなったら終わり
        while (*model->mpPointer != '\0')
        {
            //次の単語取得
            model->GetToken(); // Frame
            //}かっこの場合は終了
            if (strchr(model->mToken, '}')) break;
            //新なフレームの場合は、子フレームに追加
            if (strcmp(model->mToken, "Frame")==0)
            {
                //フレームを作成し、子フレームの配列に追加
                mChild.push_back(
                    new CModelXFrame(model));
            }
            else if (strcmp(model->mToken, "FrameTransformMatrix") == 0) 
            {
                model->GetToken(); // {
                for (int i = 0; i < mTransformMatrix.Size(); i++) 
                {
                    mTransformMatrix.M()[i] = atof(model->GetToken());
                }
                model->GetToken(); // }
            }
            else if (strcmp(model->mToken, "Mesh") == 0) 
            {
                mpMesh = new CMesh();
                mpMesh->Init(model);
            }
            else 
            {
                //上記以外の要素は読み飛ばす
                model->SkipNode();
            }

        }
    }
        //デバッグバージョンのみ有効
#ifdef _DEBUG
       
       
#endif

}
char* CModelX::Token()
{
    return mToken;
}



//コンストラクタ
CMesh::CMesh()
    : mVertexNum(0)
    , mpVertex(nullptr)
    , mFaceNum(0)
    , mpVertexIndex(nullptr)
    , mNormalNum(0)
    , mpNormal(nullptr)
    , mMaterialNum(0)
    , mMaterialIndexNum(0)
    , mpMaterialIndex(nullptr)
    , mpAnimateVertex(nullptr)
    , mpAnimateNormal(nullptr)
    , mpTextureCoords(nullptr)
{}
//デストラクタ
CMesh::~CMesh() 
{
    SAFE_DELETE_ARRAY(mpVertex);
    SAFE_DELETE_ARRAY(mpVertexIndex);
    SAFE_DELETE_ARRAY(mpNormal);
    SAFE_DELETE_ARRAY(mpMaterialIndex);
    SAFE_DELETE_ARRAY(mpAnimateVertex);
    SAFE_DELETE_ARRAY(mpAnimateNormal);
    SAFE_DELETE_ARRAY(mpTextureCoords);


    //スキンウェイトの削除
    for (size_t i = 0; i < mSkinWeights.size(); i++)
    {
        delete mSkinWeights[i];
    }

}

/*
 Init
 Meshのデータを取り込む
*/
void CMesh::Init(CModelX* model)
{
    model->GetToken();	// { or 名前
    if (!strchr(model->Token(), '{'))
    {
        //名前の場合、次が{
        model->GetToken();	// {
    }

    //頂点数の取得
    mVertexNum = atoi(model->GetToken());
    //頂点数分エリア確保
    mpVertex = new CVector[mVertexNum];
    mpAnimateVertex = new CVector[mVertexNum];
    //頂点数分データを取り込む
    for (int i = 0; i < mVertexNum; i++)
    {
        mpVertex[i].X(atof(model->GetToken()));
        mpVertex[i].Y(atof(model->GetToken()));
        mpVertex[i].Z(atof(model->GetToken()));
    }
    //面数読み込み
    mFaceNum = atoi(model->GetToken());
    //頂点数は1面に3頂点
    mpVertexIndex = new int[mFaceNum * 3];
    for (int i = 0; i < mFaceNum * 3; i += 3)
    {
        model->GetToken();	//頂点数読み飛ばし
        mpVertexIndex[i] = atoi(model->GetToken());
        mpVertexIndex[i + 1] = atoi(model->GetToken());
        mpVertexIndex[i + 2] = atoi(model->GetToken());
    }

    //単語がある間繰り返し
    while (!model->EOT())
    {
        model->GetToken();	//MeshNormals
        //}かっこの場合は終了
        if (strchr(model->Token(), '}'))

            break;

        if (strcmp(model->Token(), "MeshNormals") == 0)
        {
            model->GetToken();	// {
            //法線データ数を取得
            mNormalNum = atoi(model->GetToken());
            //法線のデータを配列に取り込む
            CVector* pNormal = new CVector[mNormalNum];
            for (int i = 0; i < mNormalNum; i++)
            {
                pNormal[i].X(atof(model->GetToken()));
                pNormal[i].Y(atof(model->GetToken()));
                pNormal[i].Z(atof(model->GetToken()));
            }
            //法線数=面数×３
            mNormalNum = atoi(model->GetToken()) * 3; //FaceNum
            int ni;
            //頂点毎に法線データを設定する
            mpNormal = new CVector[mNormalNum];
            mpAnimateNormal = new CVector[mNormalNum];


            for (int i = 0; i < mNormalNum; i += 3)
            {
                model->GetToken(); // 3
                ni = atoi(model->GetToken());
                mpNormal[i] = pNormal[ni];

                ni = atoi(model->GetToken());
                mpNormal[i + 1] = pNormal[ni];

                ni = atoi(model->GetToken());
                mpNormal[i + 2] = pNormal[ni];
            }
            delete[] pNormal;
            model->GetToken();	// }
        } // End of MeshNormals
        // MeshMaterialListのとき
        else if (strcmp(model->Token(), "MeshMaterialList") == 0)
        {
            model->GetToken(); // {
            // Materialの数
            mMaterialNum = atoi(model->GetToken());
            // FaceNum
            mMaterialIndexNum = atoi(model->GetToken());
            //マテリアルインデックスの作成
            mpMaterialIndex = new int[mMaterialIndexNum];
            for (int i = 0; i < mMaterialIndexNum; i++)
            {
                mpMaterialIndex[i] = atoi(model->GetToken());
            }
            //マテリアルデータの作成
            for (int i = 0; i < mMaterialNum; i++)
            {
                model->GetToken();	// Material
                if (strcmp(model->Token(), "Material") == 0)
                {
                    mMaterial.push_back(new CMaterial(model));
                }
                else
                {
                    // {  既出
                    model->GetToken();	//MaterialName
                    mMaterial.push_back(
                        model->FindMaterial(model->Token()));
                    model->GetToken();	// }
                }

            }
            model->GetToken();	// } //End of MeshMaterialList
        } //End of MeshMaterialList
        //SkinWeightsのとき
        else if (strcmp(model->Token(), "SkinWeights") == 0)
        {
            //CSkinWeightsクラスのインスタンスを作成し、配列に追加
            mSkinWeights.push_back(new CSkinWeights(model));
        }
        //テクスチャ座標の時
        else if (strcmp(model->Token(), "MeshTextureCoords") == 0)
        {
            model->GetToken();	// {
            //テクスチャ座標数を取得
            int textureCoordsNum = atoi(model->GetToken()) * 2;
            //テクスチャ座標のデータを配列に取り込む
            mpTextureCoords = new float[textureCoordsNum];
            for (int i = 0; i < textureCoordsNum; i++)
            {
                mpTextureCoords[i] = atof(model->GetToken());
            }
            model->GetToken();	// }
        }

        else
        {
            //以外のノードは読み飛ばし
            model->SkipNode();
        }

    }
}

/*
 Render
 画面に描画する
*/
void CMesh::Render() 
{
    /* 頂点データ，法線データの配列を有効にする */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //テクスチャマッピングの配列を有効にする
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


    /* 頂点データ，法線データの場所を指定する */
    glVertexPointer(3, GL_FLOAT, 0, mpAnimateVertex);
    glNormalPointer(GL_FLOAT, 0, mpAnimateNormal);
    glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoords);


    /* 頂点のインデックスの場所を指定して図形を描画する */
    for (int i = 0; i < mFaceNum; i++)
    {
        //マテリアルを適用する
        mMaterial[mpMaterialIndex[i]]->Enabled();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
        mMaterial[mpMaterialIndex[i]]->Disabled();

    }
    /* 頂点データ，法線データの配列を無効にする */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

/*
 Render
 メッシュが存在すれば描画する
*/
void CModelXFrame::Render() 
{
    if(mpMesh != nullptr)
        mpMesh->Render();
}

/*
Render
全てのフレームの描画処理を呼び出す
*/
void CModelX::Render() 
{
    for (size_t i = 0; i < mFrame.size(); i++) 
    {
        mFrame[i]->Render();
    }
}

bool CModelX::EOT()
{
    return *mpPointer == '\0';
}

CSkinWeights::~CSkinWeights()
{
    SAFE_DELETE_ARRAY(mpFrameName);
    SAFE_DELETE_ARRAY(mpIndex);
    SAFE_DELETE_ARRAY(mpWeight);
}

const int& CSkinWeights::FrameIndex()
{
    return mFrameIndex;
}

const CMatrix& CSkinWeights::Offset()
{
    return mOffset;
}


/*
CSkinWeights
スキンウェイトの読み込み
*/
CSkinWeights::CSkinWeights(CModelX* model)
    : mpFrameName(nullptr)
    , mFrameIndex(0)
    , mIndexNum(0)
    , mpIndex(nullptr)
    , mpWeight(nullptr)
{
    model->GetToken();	// {
    model->GetToken();	// FrameName
    //フレーム名エリア確保、設定
    mpFrameName = new char[strlen(model->Token()) + 1];
    strcpy(mpFrameName, model->Token());

    //頂点番号数取得
    mIndexNum = atoi(model->GetToken());

    //頂点番号数が0を超える
    if (mIndexNum > 0) {
        //頂点番号と頂点ウェイトのエリア確保
        mpIndex = new int[mIndexNum];
        mpWeight = new float[mIndexNum];
        //頂点番号取得
        for (int i = 0; i < mIndexNum; i++)
            mpIndex[i] = atoi(model->GetToken());
        //頂点ウェイト取得
        for (int i = 0; i < mIndexNum; i++)
            mpWeight[i] = atof(model->GetToken());
    }
    //オフセット行列取得
    for (int i = 0; i < 16; i++) {
        mOffset.M()[i] = atof(model->GetToken());
    }
    model->GetToken();	// }

}


CAnimationSet::~CAnimationSet()
{
    SAFE_DELETE_ARRAY(mpName);
    //アニメーション要素の削除
    for (size_t i = 0; i < mAnimation.size(); i++) 
    {
        delete mAnimation[i];
        
    }
 

}

/*
CAnimationSet
*/
CAnimationSet::CAnimationSet(CModelX* model)
    : mpName(nullptr)
    , mTime(0)
    , mWeight(0)
    , mMaxTime(0)
{
    model->mAnimationSet.push_back(this);
    model->GetToken();	// Animation Name
    //アニメーションセット名を退避
    mpName = new char[strlen(model->Token()) + 1];
    strcpy(mpName, model->Token());
    model->GetToken(); // {
    while (!model->EOT()) 
    {
        model->GetToken(); // } or Animation
        if (strchr(model->Token(), '}'))break;
        if (strcmp(model->Token(), "Animation") == 0) 
        {
            //Animation要素読み込み
            mAnimation.push_back(new CAnimation(model));

        }
    }
    //終了時間設定
    mMaxTime = mAnimation[0]->mpKey[mAnimation[0]->mKeyNum - 1].mTime;
#ifdef _DEBUG
  
#endif
}

CAnimation::~CAnimation() 
{
    SAFE_DELETE_ARRAY(mpFrameName);
    SAFE_DELETE_ARRAY(mpKey);

}


/*
 FindFrame(フレーム名)
 フレーム名に該当するフレームのアドレスを返す
*/
CModelXFrame* CModelX::FindFrame(char* name) 
{
    //イテレータの作成
    std::vector<CModelXFrame*>::iterator itr;
    //先頭から最後まで繰り返す
    for (itr = mFrame.begin(); itr != mFrame.end(); itr++) 
    {
        //名前が一致したか？
        if (strcmp(name, (*itr)->mpName) == 0) 
        {
            //一致したらそのアドレスを返す
            return *itr;
        }
    }
    //一致するフレームが無い場合はnullptrを返す
    return nullptr;
}


CAnimation::CAnimation(CModelX* model)
    : mpFrameName(nullptr)
    , mFrameIndex(0)
    , mKeyNum(0)
    , mpKey(nullptr)

{
    model->GetToken(); // { or Animation Name
    if (strchr(model->Token(), '{')) 
    {
        model->GetToken(); // {
    }
    else {
        model->GetToken(); // {
        model->GetToken(); // {
    }

    model->GetToken(); //FrameName
    mpFrameName = new char[strlen(model->Token()) + 1];
    strcpy(mpFrameName, model->Token());
    mFrameIndex =
        model->FindFrame(model->Token())->Index();
    model->GetToken(); // }
    //キーの配列を保存しておく配列
    CMatrix* key[4] = { nullptr, nullptr, nullptr, nullptr };
    //時間の配列を保存しておく配列
    float* time[4] = { nullptr, nullptr, nullptr, nullptr };
    while (!model->EOT()) 
    {
        model->GetToken(); // } or AnimationKey
        if (strchr(model->Token(), '}')) break;
        if (strcmp(model->Token(), "AnimationKey") == 0)
        {
            model->GetToken(); // {
        //データのタイプ取得
            int type = atoi(model->GetToken());
            //時間数取得
            mKeyNum = atoi(model->GetToken());
            switch (type) 
            {
             case 0: // Rotation Quaternion
             //行列の配列を時間数分確保
                key[type] = new CMatrix[mKeyNum];
                //時間の配列を時間数分確保
                time[type] = new float[mKeyNum];
                //時間数分繰り返す
                for (int i = 0; i < mKeyNum; i++) 
                {
                    //時間取得
                    time[type][i] = atof(model->GetToken());
                    model->GetToken(); // 4を読み飛ばし
                    //w,x,y,zを取得
                    float w = atof(model->GetToken());
                    float x = atof(model->GetToken());
                    float y = atof(model->GetToken());
                    float z = atof(model->GetToken());
                    //クォータニオンから回転行列に変換
                    key[type][i].Quaternion(x, y, z, w);
                }
                break;
             case 1: //拡大・縮小の行列作成
                 key[type] = new CMatrix[mKeyNum];
                 time[type] = new float[mKeyNum];
                 for (int i = 0; i < mKeyNum; i++) 
                 {
                     time[type][i] = atof(model->GetToken());
                     model->GetToken(); // 3
                     float x = atof(model->GetToken());
                     float y = atof(model->GetToken());
                     float z = atof(model->GetToken());
                     key[type][i].Scale(x, y, z);
                 }
                 break;
             case 2: //移動の行列作成
                 key[type] = new CMatrix[mKeyNum];
                 time[type] = new float[mKeyNum];
                 for (int i = 0; i < mKeyNum; i++) 
                 {
                     time[type][i] = atof(model->GetToken());
                     model->GetToken(); // 3
                     float x = atof(model->GetToken());
                     float y = atof(model->GetToken());
                     float z = atof(model->GetToken());
                     key[type][i].Translate(x, y, z);
                 }
                 break;
             case 4: //行列データを取得
                 mpKey = new CAnimationKey[mKeyNum];
                 for (int i = 0; i < mKeyNum; i++) 
                 {
                     mpKey[i].mTime = atof(model->GetToken()); // Time
                     model->GetToken(); // 16
                     for (int j = 0; j < 16; j++) 
                     {
                         mpKey[i].mMatrix.M()[j] = atof(model->GetToken());
                     }
                 }
                 break;
            }
            model->GetToken(); // }
        }
        else 
        {
            model->SkipNode();

           
        }
    }

    //行列データではない時
    if (mpKey == nullptr) 
    {
        //時間数分キーを作成
        mpKey = new CAnimationKey[mKeyNum];
        for (int i = 0; i < mKeyNum; i++) 
        {
            //時間設定
            mpKey[i].mTime = time[2][i]; // Time
            //行列作成 Scale * Rotation * Position
            mpKey[i].mMatrix = key[1][i] * key[0][i] * key[2][i];
        }
    }
    //確保したエリア解放
    for (int i = 0; i < ARRAY_SIZE(key); i++) 
    {
        SAFE_DELETE_ARRAY(time[i]);
        SAFE_DELETE_ARRAY(key[i]);
    }

#ifdef _DEBUG
 
#endif
}

void CAnimationSet::Time(float time)
{
    mTime = time;
}
void CAnimationSet::Weight(float weight)
{
    mWeight = weight;
}

std::vector<CAnimationSet*>& CModelX::AnimationSet()
{
    return mAnimationSet;
}

void CAnimationSet::AnimateMatrix(CModelX* model)
{
    //重みが0は飛ばす
    if (mWeight == 0) return;
    //フレーム分（Animation分）繰り返す
    for (size_t j = 0; j < mAnimation.size(); j++) 
    {
        //フレームを取得する
        CAnimation* animation = mAnimation[j];
        //キーがない場合は次のアニメーションへ
        if (animation->mpKey == nullptr) continue;
        //該当するフレームの取得
        CModelXFrame* frame = model->mFrame[animation->mFrameIndex];
        //最初の時間より小さい場合
        if (mTime < animation->mpKey[0].mTime) 
        {
            //変換行列を0コマ目の行列で更新
            frame->mTransformMatrix += animation->mpKey[0].mMatrix * mWeight;
        }
        //最後の時間より大きい場合
        else if (mTime >= animation->mpKey[animation->mKeyNum - 1].mTime) {
            //変換行列を最後のコマの行列で更新
            frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * mWeight;
        }
        else 
        {
            //時間の途中の場合
            for (int k = 1; k < animation->mKeyNum; k++) 
            {
                //変換行列を、線形補間にて更新
                if (mTime < animation->mpKey[k].mTime &&
                    animation->mpKey[k - 1].mTime != animation->mpKey[k].mTime) 
                {

                    float r = (animation->mpKey[k].mTime - mTime) /
                        (animation->mpKey[k].mTime - animation->mpKey[k - 1].mTime);

                    frame->mTransformMatrix +=
                        (animation->mpKey[k - 1].mMatrix * r +
                            animation->mpKey[k].mMatrix * (1 - r)) * mWeight;
                    break;
                }
            }
        }
    }
}

std::vector<CAnimation*>& CAnimationSet::Animation()
{
    return mAnimation;
}


/*
AnimateFrame
フレームの変換行列をアニメーションデータで更新する
*/
void CModelX::AnimateFrame() 
{
    //アニメーションで適用されるフレームの
    //変換行列をゼロクリアする
    for (size_t i = 0; i < mAnimationSet.size(); i++) 
    {
        CAnimationSet* animSet = mAnimationSet[i];
        //重みが0は飛ばす
        if (animSet->mWeight == 0) continue;
        //フレーム分（Animation分）繰り返す
        for (size_t j = 0;
            j < animSet->Animation().size(); j++)
        {
            CAnimation* animation =
                animSet->Animation()[j];
            //該当するフレームの変換行列をゼロクリアする
            memset(
                &mFrame[animation->mFrameIndex]
                ->mTransformMatrix,
                0, sizeof(CMatrix));
        }
    }
    //アニメーションに該当するフレームの変換行列を
    //アニメーションのデータで設定する
    for (size_t i = 0; i < mAnimationSet.size(); i++) 
    {
        CAnimationSet* animSet = mAnimationSet[i];
        //重みが0は飛ばす
        if (animSet->mWeight == 0) continue;
        animSet->AnimateMatrix(this);
    }
#ifdef _DEBUG

#endif
}

/*
 AnimateCombined
 合成行列の作成
*/
void CModelXFrame::AnimateCombined(CMatrix* parent) 
{
    //自分の変換行列に、親からの変換行列を掛ける
    mCombinedMatrix = mTransformMatrix * (*parent);
    //子フレームの合成行列を作成する
    for (size_t i = 0; i < mChild.size(); i++) 
    {
        mChild[i]->AnimateCombined(&mCombinedMatrix);
    }
#ifdef _DEBUG
    //printf("Frame::%s\n", mpName);
   // mCombinedMatrix.Print();
#endif
}

std::vector<CModelXFrame*>& CModelX::Frames()
{
    return mFrame;
}

void CMesh::SetSkinWeightFrameIndex(CModelX* model)
{
    //スキンウェイト分繰り返し
    for (size_t i = 0; i < mSkinWeights.size(); i++) 
    {
        //フレーム名のフレームを取得する
        CModelXFrame* frame = model->FindFrame(mSkinWeights[i]->mpFrameName);
        //フレーム番号を設定する
        mSkinWeights[i]->mFrameIndex = frame->Index();
    }
}


void CModelX::SetSkinWeightFrameIndex() 
{
    //フレーム数分繰り返し
    for (size_t i = 0; i < mFrame.size(); i++) 
    {
        //メッシュがあれば
        if (mFrame[i]->mpMesh != nullptr) 
        {
            mFrame[i]->mpMesh->SetSkinWeightFrameIndex(this);
        }
    }
}

void CMesh::AnimateVertex(CModelX* model) 
{
    //アニメーション用の頂点エリアクリア
    memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
    memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
    //スキンウェイト分繰り返し
    for (size_t i = 0; i < mSkinWeights.size(); i++) 
    {
        //フレーム番号取得
        int frameIndex = mSkinWeights[i]->mFrameIndex;
        //オフセット行列とフレーム合成行列を合成
        CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * model->Frames()[frameIndex]->CombinedMatrix();
        //頂点数分繰り返し
        for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++) 
        {
            //頂点番号取得
            int index = mSkinWeights[i]->mpIndex[j];
            //重み取得
            float weight = mSkinWeights[i]->mpWeight[j];
            //頂点と法線を更新する
            mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
            mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
        }
    }
    //法線を正規化する
    for (int i = 0; i < mNormalNum; i++) 
    {
        mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
    }
}

/*
AnimateVertex
頂点にアニメーションを適用する
*/
void CModelX::AnimateVertex() 
{
    //フレーム数分繰り返し
    for (size_t i = 0; i < mFrame.size(); i++) 
    {
        //メッシュに面があれば
        if (mFrame[i]->mpMesh != nullptr) 
        {
            //頂点をアニメーションで更新する
            mFrame[i]->mpMesh->AnimateVertex(this);
        }
    }
}



const CMatrix& CModelXFrame::CombinedMatrix()
{
    return mCombinedMatrix;
}

float CAnimationSet::Time()
{
    return mTime;
}

float CAnimationSet::MaxTime()
{
    return mMaxTime;
}

CMaterial* CModelX::FindMaterial(char* name) 
{
    //マテリアル配列のイテレータ作成
    std::vector<CMaterial*>::iterator itr;
    //マテリアル配列を先頭から順に検索
    for (itr = mMaterial.begin(); itr != mMaterial.end(); itr++) 
    {
        //名前が一致すればマテリアルのポインタを返却
        if (strcmp(name, (*itr)->Name()) == 0) 
        {
            return *itr;
        }
    }
    //無い時はnullptrを返却
    return nullptr;
}

std::vector<CMaterial*>& CModelX::Material()
{
    return mMaterial;
}


void CModelX::AnimateVertex(CMatrix* mat) 
{
    //フレーム数分繰り返し
    for (size_t i = 0; i < mFrame.size(); i++) 
    {
        //メッシュがあれば
        if (mFrame[i]->mpMesh) 
        {
            //頂点をアニメーションで更新する
            mFrame[i]->
                mpMesh->AnimateVertex(mat);
        }
    }
}

void CMesh::AnimateVertex(CMatrix* mat) {
    //アニメーション用の頂点エリアクリア
    memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
    memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
    //スキンウェイト分繰り返し
    for (size_t i = 0; i < mSkinWeights.size(); i++) {
        //フレーム番号取得
        int frameIndex = mSkinWeights[i]->mFrameIndex;
        //フレーム合成行列にオフセット行列を合成
        CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * mat[frameIndex];
        //頂点数分繰り返し
        for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++) {
            //頂点番号取得
            int index = mSkinWeights[i]->mpIndex[j];
            //重み取得
            float weight = mSkinWeights[i]->mpWeight[j];
            //頂点と法線を更新する
            mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
            mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
        }
    }
    //法線を正規化する
    for (int i = 0; i < mNormalNum; i++) {
        mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
    }
}
