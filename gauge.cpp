//=====================================================================
//
// Gauge [Gauge.cpp]
// Author : Tenma Saito
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Gauge.h"
#include "Player.h"
#include "LevelGenerator.h"
#include "util.h"
#include "input.h"
#include "sound.h"
#include "font.h"
#include "scene.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define RELEASE(p) do{ if(p != nullptr){ (p)->Release(); (p) = nullptr;} }while(0)				// バッファ解放
#define RELEASE_ARRAY(pp, num) do{ for(int i = 0; i < num; i++){ RELEASE(pp[i]); } }while(0)	// 配列バッファ解放
#define TEXTURE_NUM			(1)		// テクスチャ数

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 apTex[TEXTURE_NUM];	// テクスチャバッファへのポインタ
	bool bSafeVtx;		// 頂点バッファの獲得状況
}GAUGEBUFFER;

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
bool CreateGaugeBuffer(_Out_ GAUGEBUFFER *pOut);	// バッファ作成
void SetVertexGauge(void);							// 頂点設定
void SetStateGauge(LPGAUGE pGauge, GAUGESTATE state);				// 状態設定
void UpdateStateGauge(LPGAUGE pGauge);				// 状態更新
void CalcGauge(LPGAUGE pGauge);						// ゲージの計算
void SetVertexDefGauge(void);						// 裏ゲージの頂点設定
void SetVertexPosUnFollow(VERTEX_2D *pVtx, BASEOBJECT obj);	// 追従無しの頂点設定
void SetTextureAnim(VERTEX_2D *pVtx, float f, int nCounter, float spd);	// アニメーションテクスチャの設定

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
// --- static const変数の定義 --- //
const D3DXVECTOR3 GAUGE_CONST::DefPosition = D3DXVECTOR3(0, SCREEN_HEIGHT, 0);
const D3DXVECTOR2 GAUGE_CONST::DefSize = D3DXVECTOR2(SCREEN_WIDTH, 40.0f);
const int GAUGE_CONST::aStateCount[GAUGESTATE_MAX] =
{
	0,		// null
	0,		// チャージ中
	120,	// チャージ完了(完了カウント)
	120,	// 解放中
};
const int GAUGE_CONST::nGaugePhase = 2;			// ゲージの段階(0段階 -> 1段階)
const float GAUGE_CONST::fAnimSpeed = -0.01f;	// アニメーションスピード

// --- const変数宣言 --- //
const char *c_apTextureGauge[TEXTURE_NUM] =
{
	"data/TEXTURE/StarAnimation_sub.png",
};

Gauge g_gauge = {};					// ゲージの情報
GAUGEBUFFER g_gaugeBuffer = {};		// ゲージのバッファ
FONT* g_pFontGaugePress = NULL;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGauge(void)
{
	// 初期化
	ZeroMemory(GetGaugePtr(), sizeof(Gauge));

	LPGAUGE pGauge = GetGaugePtr();
	pGauge->obj.color = D3DXCOLOR_WHITE;
	pGauge->obj.bVisible = true;
	SetStateGauge(pGauge, GAUGESTATE_CHARGE);

	// バッファ作成
	CreateGaugeBuffer(&g_gaugeBuffer);

	g_pFontGaugePress = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, SCREEN_VCENTER + 210, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 0, 1.0f),
		71,
		"- PRESS [MOUSE LEFT] OR [A]! -",
		DT_CENTER | DT_TOP
	);
	g_pFontGaugePress->obj.bVisible = false;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGauge(void)
{
	// 頂点破棄
	RELEASE(g_gaugeBuffer.pVtxBuff);

	// テクスチャ破棄
	RELEASE_ARRAY(g_gaugeBuffer.apTex, TEXTURE_NUM);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGauge(void)
{
	LPGAUGE pGauge = GetGaugePtr();

	if (
		(GetMouseTrigger(MOUSE_LEFT) ||
		GetJoypadTrigger(JOYKEY_A)) &&
		pGauge->state == GAUGESTATE_FULLCHARGE)
	{
		PlaySound(SOUND_LABEL_SE_BURST);
		SetStateGauge(pGauge, GAUGESTATE_BURST);
	}

	// 状態更新
	UpdateStateGauge(pGauge);

	// ゲージの計算
	CalcGauge(pGauge);

	// 裏の頂点設定
	SetVertexDefGauge();

	// 頂点設定
	SetVertexGauge();

	if (GetCurrentScene() == SCENE_GAME)
	{
		if (pGauge->state == GAUGESTATE_FULLCHARGE)
		{
			if (pGauge->nCounterState % 2 == 0)
			{
				g_pFontGaugePress->obj.bVisible ^= 1;
			}
		}
		else
		{
			g_pFontGaugePress->obj.bVisible = false;
		}
	}

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGauge(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPGAUGE pGauge = GetGaugePtr();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_gaugeBuffer.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < GAUGE_CONST::nGaugePhase; nCntGauge++)
	{
		if (pGauge->obj.bVisible)
		{// 表示状態
			LPDIRECT3DTEXTURE9 tex = NULL;
			if (nCntGauge == 1) tex = g_gaugeBuffer.apTex[0];

			// テクスチャの設定
			pDevice->SetTexture(0, tex);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGauge, 2);
		}
	}

	DrawFont(g_pFontGaugePress);
}

//=====================================================================
// ポインタ取得
//=====================================================================
LPGAUGE GetGaugePtr(void)							
{
	return &g_gauge;
}

//=====================================================================
// バッファ作成
//=====================================================================
bool CreateGaugeBuffer(_Out_ GAUGEBUFFER *pOut)
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
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (4 * GAUGE_CONST::nGaugePhase),
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
	for (int nCntTexture = 0; nCntTexture < TEXTURE_NUM; nCntTexture++)
	{
		// NULLCHECK
		if (c_apTextureGauge[nCntTexture] == nullptr)
		{
			pOut->apTex[nCntTexture] = NULL;
			continue;
		}

		// バッファ作成
		hr = D3DXCreateTextureFromFile(pDevice,
			c_apTextureGauge[nCntTexture],
			&pOut->apTex[nCntTexture]);
	}

	return true;
}

//=====================================================================
// 頂点設定
//=====================================================================
void SetVertexGauge(void)							
{
	VERTEX_2D *pVtx = NULL;					// 頂点情報へのポインタ
	LPGAUGE pGauge = GetGaugePtr();		// 弾のポインタ取得

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_gaugeBuffer.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4;

	if (pGauge->state == GAUGESTATE_FULLCHARGE)
	{
		if (pGauge->bAlpha)
		{
			pGauge->obj.color.a -= 0.03f;
			if (pGauge->obj.color.a <= 0.0f)
			{
				pGauge->obj.color.a = 0.0f;
				pGauge->bAlpha = false;
			}
		}
		else
		{
			pGauge->obj.color.a += 0.03f;
			if (pGauge->obj.color.a >= 1.0f)
			{
				pGauge->obj.color.a = 1.0f;
				pGauge->bAlpha = true;
			}
		}
	}

	float fAnimSpeed = GAUGE_CONST::fAnimSpeed;
	if (pGauge->state == GAUGESTATE_BURST)
	{
		fAnimSpeed += fAnimSpeed;
		pGauge->obj.color.a = 1.0f;
	}

	// 頂点情報を設定
	SetVertexPosUnFollow(pVtx, pGauge->obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, pGauge->obj.color);

	SetTextureAnim(pVtx, pGauge->fLerp, pGauge->nAnimationCounter, fAnimSpeed);

	// 頂点バッファをアンロック
	g_gaugeBuffer.pVtxBuff->Unlock();
}

//=====================================================================
// 状態設定
//=====================================================================
void SetStateGauge(LPGAUGE pGauge, GAUGESTATE state)
{
	// NULLCHECK
	if (pGauge == nullptr) return;

	// 状態を保存し、そのカウントに設定
	pGauge->state = state;
	pGauge->nCounterState = GAUGE_CONST::aStateCount[state];
}

//=====================================================================
// 状態更新
//=====================================================================
void UpdateStateGauge(LPGAUGE pGauge)				
{
	// NULLCHECK
	if (pGauge == nullptr) return;

	switch (pGauge->state)
	{
	case GAUGESTATE_NONE:
		break;

	case GAUGESTATE_CHARGE:

		pGauge->nAnimationCounter++;

		break;

	case GAUGESTATE_FULLCHARGE:

		pGauge->nAnimationCounter++;

		break;

	case GAUGESTATE_BURST:

		pGauge->nAnimationCounter--;
		pGauge->nCounterState--;
		if (pGauge->nCounterState <= 0)
		{
			SetStateGauge(pGauge, GAUGESTATE_CHARGE);
		}

		break;

	default:
		break;
	}
}

//=====================================================================
// ゲージ変動処理
//=====================================================================
void AddGauge(int nValue)
{
	LPGAUGE pGauge = GetGaugePtr();		// 弾のポインタ取得

	// NULLCHECK
	if (pGauge == nullptr) return;

	pGauge->nCounterState += nValue;
	if (pGauge->nCounterState >= GAUGE_CONST::aStateCount[GAUGESTATE_FULLCHARGE])
	{
		// 状態更新
		SetStateGauge(pGauge, GAUGESTATE_FULLCHARGE);
	}
}

//=====================================================================
// ゲージ計算処理
//=====================================================================
void CalcGauge(LPGAUGE pGauge)
{
	// NULLCHECK
	if (pGauge == nullptr) return;

	// ゲージの値を0～1の範囲に変換
	pGauge->fLerp = (float)pGauge->nCounterState / (float)GAUGE_CONST::aStateCount[GAUGESTATE_FULLCHARGE];

	// ゲージの値をラープで計算
	D3DXVECTOR3 posNone = GAUGE_CONST::DefPosition;
	posNone.x = 0.0f;	// Yのみに変換
	posNone.z = 0.0f;
	D3DXVECTOR3 Value = Lerp(posNone, Vector2To3(GAUGE_CONST::DefSize), pGauge->fLerp);
	pGauge->obj.size = Value;
}

//=====================================================================
// 追従無しの頂点設定
//=====================================================================
void SetVertexPosUnFollow(VERTEX_2D *pVtx, BASEOBJECT obj)
{
	pVtx[0].pos.x = 0;
	pVtx[0].pos.y = SCREEN_HEIGHT - GAUGE_CONST::DefSize.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = obj.size.x;
	pVtx[1].pos.y = SCREEN_HEIGHT - GAUGE_CONST::DefSize.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = 0;
	pVtx[2].pos.y = SCREEN_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = obj.size.x;
	pVtx[3].pos.y = SCREEN_HEIGHT;
	pVtx[3].pos.z = 0.0f;
}

//=====================================================================
// 裏ゲージの頂点設定
//=====================================================================
void SetVertexDefGauge(void)
{
	VERTEX_2D* pVtx = NULL;					// 頂点情報へのポインタ
	LPGAUGE pGauge = GetGaugePtr();		// 弾のポインタ取得

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_gaugeBuffer.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = 0;
	pVtx[0].pos.y = SCREEN_HEIGHT - GAUGE_CONST::DefSize.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = SCREEN_WIDTH;
	pVtx[1].pos.y = SCREEN_HEIGHT - GAUGE_CONST::DefSize.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = 0;
	pVtx[2].pos.y = SCREEN_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = SCREEN_WIDTH;
	pVtx[3].pos.y = SCREEN_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	SetVertexRHW(pVtx, 1.0f);
	if (GetGaugePtr()->state == GAUGESTATE_FULLCHARGE)
	{
		SetVertexColor(pVtx, D3DXCOLOR_GRAY(1.0f));
	}
	else
	{
		SetVertexColor(pVtx, D3DXCOLOR_GRAY(0.3f));
	}
	SetVertexTexturePos(pVtx, pGauge->obj.bInversed);

	// 頂点バッファをアンロック
	g_gaugeBuffer.pVtxBuff->Unlock();
}

//=====================================================================
// アニメーションテクスチャの設定
//=====================================================================
void SetTextureAnim(VERTEX_2D *pVtx, float f, int nCounter, float spd)
{
	float fStart = spd * nCounter;
	float fEnd = f + fStart;

	pVtx[0].tex.x = fStart;
	pVtx[0].tex.y = 0.0f;

	pVtx[1].tex.x = fEnd;
	pVtx[1].tex.y = 0.0f;

	pVtx[2].tex.x = fStart;
	pVtx[2].tex.y = 1.0f;

	pVtx[3].tex.x = fEnd;
	pVtx[3].tex.y = 1.0f;
}