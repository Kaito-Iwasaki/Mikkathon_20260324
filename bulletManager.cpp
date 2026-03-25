//=====================================================================
//
// bulletManager [bulletManager.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "bulletManager.h"
#include "bulletController.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define RELEASE(p) do{ if(p != nullptr){ (p)->Release(); (p) = nullptr;} }while(0)				// バッファ解放
#define RELEASE_ARRAY(pp, num) do{ for(int i = 0; i < num; i++){ RELEASE(pp[i]); } }while(0)	// 配列バッファ解放
#ifdef _DEBUG
#define DEBUG_FUNC(func)	func			// DEBUG関数
#else
#define DEBUG_FUNC(func)	printf("")		// DEBUG関数
#endif

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 apTex[BT_MAX];	// テクスチャバッファへのポインタ
	bool bSafeVtx;		// 頂点バッファの獲得状況
}BULLETBUFFER;

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
bool CreateBulletBuffer(_Out_ BULLETBUFFER *pOut);	// バッファ作成
void SetVertexBullet(void);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
const int BULLET_CONST::nBulletMax = 64;	// 弾の最大数
const char *c_apTextureBullet[BT_MAX] =
{
	"data\\TEXTURE\\punch.png",
};

Bullet g_aBullet[BULLET_CONST::nBulletMax];	// 弾の情報
BULLETBUFFER g_bufferBullet;	// 弾のバッファ
 
//=====================================================================
// 初期化処理
//=====================================================================
void InitBulletManager(void)
{
	// 初期化処理
	ZeroMemory(&g_aBullet[0], sizeof(g_aBullet));

	// バッファ作成
	CreateBulletBuffer(&g_bufferBullet);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBulletManager(void)
{
	RELEASE(g_bufferBullet.pVtxBuff);

	RELEASE_ARRAY(g_bufferBullet.apTex, BT_MAX);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBulletManager(void)
{
	LPBULLET pBullet = GetBulletPtr();	// 弾のポインタ取得

	for (int nCntBullet = 0; nCntBullet < BULLET_CONST::nBulletMax; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false) continue;

		// 弾を更新
		BulletController(pBullet);
	}
	
	// 頂点バッファを更新
	SetVertexBullet();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBulletManager(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPBULLET pBullet = GetBulletPtr();	// 弾のポインタ取得

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_bufferBullet.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntBullet = 0; nCntBullet < BULLET_CONST::nBulletMax; nCntBullet++, pBullet++)
	{
		if (pBullet->obj.bVisible && pBullet->bUse)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, g_bufferBullet.apTex[pBullet->type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);
		}
	}
}

//=====================================================================
// 取得処理
//=====================================================================
LPBULLET GetBulletPtr(void)
{
	return &g_aBullet[0];
}

//=====================================================================
// バッファ作成処理
//=====================================================================
bool CreateBulletBuffer(_Out_ BULLETBUFFER *pOut)
{
	// NULLCHECK
	if (pOut == nullptr) return false;

	// 初期化
	*pOut = {};

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (pDevice == nullptr) return false;	// NULLCHECK

	HRESULT hr;		// エラーコード

	// 頂点バッファ確保
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (4 * BULLET_CONST::nBulletMax),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pOut->pVtxBuff,
		NULL);
	
	// NULLCHECK
	if (pOut->pVtxBuff == nullptr)
	{
		pOut->bSafeVtx = false;		// 作成失敗
		return false;
	}

	// テクスチャ読み込み
	for (int nCntTexture = 0; nCntTexture < BT_MAX; nCntTexture++)
	{
		// NULLCHECK
		if (c_apTextureBullet[nCntTexture] == nullptr)
		{
			pOut->apTex[nCntTexture] = NULL;
			continue;
		}

		// バッファ作成
		hr = D3DXCreateTextureFromFile(pDevice,
			c_apTextureBullet[nCntTexture],
			&pOut->apTex[nCntTexture]);
	}

	return true;
}

//=====================================================================
// 頂点バッファ設定処理
//=====================================================================
void SetVertexBullet(void)
{
	VERTEX_2D *pVtx = NULL;					// 頂点情報へのポインタ
	LPBULLET pBullet = GetBulletPtr();		// 弾のポインタ取得

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_bufferBullet.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < BULLET_CONST::nBulletMax; nCntBullet++, pBullet++)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pBullet->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pBullet->obj.color);
		SetVertexTexturePos(pVtx, pBullet->obj.bInversed);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_bufferBullet.pVtxBuff->Unlock();
}