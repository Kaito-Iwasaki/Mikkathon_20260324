//=====================================================================
// 
// effect [effect.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================
#include "effect.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_SIZE	D3DXVECTOR3(64.0f, 64.0f, 0.0f)
#define INIT_COLOR	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f)
#define INIT_SCALE	(1.0f)

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
EFFECT g_aEffect[MAX_EFFECT];

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	EFFECT* pEffect = &g_aEffect[0];

	// 構造体の初期化
	memset(pEffect, 0, sizeof(EFFECT) * MAX_EFFECT);
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++, pEffect++)
	{
		pEffect->obj.size = INIT_SIZE;
		pEffect->obj.color = INIT_COLOR;
		pEffect->fScale = INIT_SCALE;
		pEffect->bUsed = false;
		pEffect->obj.bVisible = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitEffect(void)
{
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	EFFECT* pEffect = &g_aEffect[0];

	for (int nCount = 0; nCount < MAX_EFFECT; nCount++, pEffect++)
	{
		if (pEffect->bUsed == false) continue;

		if (IsObjectOutOfScreen(pEffect->obj) || pEffect->nLife < 1)
		{
			pEffect->bUsed = false;
			continue;
		}

		pEffect->obj.pos += D3DXVECTOR3(sinf(pEffect->fAngle), cosf(pEffect->fAngle), 0.0f) * pEffect->info.fSpeed;
		pEffect->fScale = pEffect->info.fMaxScale * ((float)pEffect->nLife / (float)pEffect->info.nMaxLife);
		pEffect->obj.rot.z += pEffect->info.fRotSpeed;
		pEffect->nLife--;
		pEffect->obj.color.a = pEffect->info.fMaxAlpha * ((float)pEffect->nLife / (float)pEffect->info.nMaxLife);
	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	VERTEX_2D* pVtx;
	EFFECT* pEffect = &g_aEffect[0];

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_EFFECT; nCount++, pEffect++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pEffect->obj.pos, pEffect->obj.size * pEffect->fScale, pEffect->obj.rot.z);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pEffect->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//// 加算合成を適用
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pEffect = &g_aEffect[0];
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++, pEffect++)
	{
 		if (pEffect->bUsed == false) continue;
		if (pEffect->obj.bVisible == false) continue;

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}

	//// 加算合成を解除
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void SetEffect(D3DXVECTOR3 pos, float fSpeed, float fRotSpeed, float fAngle, float fScale, int nLife, D3DXCOLOR col)
{
	EFFECT* pEffect = &g_aEffect[0];

	for (int nCount = 0; nCount < MAX_EFFECT; nCount++, pEffect++)
	{
		if (pEffect->bUsed == true) continue;

		pEffect->obj.pos = pos;
		pEffect->obj.size = INIT_SIZE * fScale;
 		pEffect->obj.color = col;

		pEffect->info.fSpeed = fSpeed;
		pEffect->info.fRotSpeed = fRotSpeed;
		pEffect->fAngle = fAngle;
		pEffect->fScale = fScale;
		pEffect->info.fMaxScale = fScale;
		pEffect->nLife = nLife;
		pEffect->info.nMaxLife = nLife;
		pEffect->info.fMaxAlpha = col.a;
		pEffect->bUsed = true;
		pEffect->obj.bVisible = true;

		break;
	}
}