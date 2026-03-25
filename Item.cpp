//=====================================================================
//
// Item [Item.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Item.h"
#include "Player.h"
#include "itemGenerator.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define ToVector3(Vec2)		D3DXVECTOR3(Vec2.x, Vec2.y, 0.0f)		// ベクトルの昇格変換
#define RELEASE(p) do{ if(p != nullptr){ (p)->Release(); (p) = nullptr;} }while(0)				// バッファ解放
#define RELEASE_ARRAY(pp, num) do{ for(int i = 0; i < num; i++){ RELEASE(pp[i]); } }while(0)	// 配列バッファ解放(配列の先頭アドレス, 配列数)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 apTex[ITEMTYPE_MAX];	// テクスチャバッファへのポインタ
	bool bSafeVtx;		// 頂点バッファの獲得状況
}ITEMBUFFER;

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
void CollisionItem(LPITEM pItem);					// 当たり判定処理
void HitItem(LPITEM pItem, PLAYER *pPlayer);		// 獲得時の処理
void SetItemState(LPITEM pItem, ITEMSTATE state);	// ステート設定
void UpdateItemState(LPITEM pItem);					// ステートの更新
void RemoveItem(LPITEM pItem);						// アイテムの消去処理
bool CreateItemBuffer(_Out_ ITEMBUFFER *pOut);		// バッファ作成
void SetVertexItem(void);							// 頂点設定

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
// --- static const変数の値設定 --- //
const int ITEM_CONST::nMaxItem = 256;		// アイテムの最大数
const unsigned int ITEM_CONST::aStateCount[ITEMSTATE_MAX] = 
{
	0,
	INFINITE,
	30,
	0
};

const D3DXVECTOR2 ITEM_CONST::DefSize = D3DXVECTOR2(100, 100);	// 基本サイズ
const D3DXCOLOR ITEM_CONST::DefColor = D3DXCOLOR(1, 1, 1, 1);	// 基本色

// --- const変数の宣言 --- //
const char *c_apTextureItem[ITEMTYPE_MAX] =	// テクスチャパス
{
	"data/TEXTURE/ProteinNormal.png",	
};

// --- 通常のグローバル変数の宣言 --- //
ITEMBUFFER g_itemBuffer = {};				// アイテムのバッファ
Item g_aItem[ITEM_CONST::nMaxItem] = {};	// アイテムの情報

//=====================================================================
// 初期化処理
//=====================================================================
void InitItem(void)
{
	LPITEM pItem = GetItemPtr();			// アイテムへのポインタ
	ITEMBUFFER *pBuffer = &g_itemBuffer;	// バッファ構造体へのポインタ

	// 初期化
	ZeroMemory(pItem, sizeof(Item) * ITEM_CONST::nMaxItem);
	ZeroMemory(pBuffer, sizeof(ITEMBUFFER));

	// 各初期値を設定
	for (int nCntItem = 0; nCntItem < ITEM_CONST::nMaxItem; nCntItem++, pItem++)
	{
		pItem->obj.bVisible = true;
		pItem->obj.color = ITEM_CONST::DefColor;
		pItem->obj.size = ToVector3(ITEM_CONST::DefSize);
	}

	// バッファ作成
	CreateItemBuffer(&g_itemBuffer);

	ITEM_GENERATE_SETTING igs;
	igs.nFrameSpawn = 240;
	igs.nUpperSpawn = 10;
	igs.nLowerSpawn = 3;
	igs.rect = FLOAT_RECT{ -500, -500, 2000, 2000 };

	// アイテム発生処理の設定
	SetItemGenerator(igs);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitItem(void)
{
	// 頂点バッファ解放
	RELEASE(g_itemBuffer.pVtxBuff);

	// テクスチャバッファ解放
	RELEASE_ARRAY(g_itemBuffer.apTex, ITEMTYPE_MAX);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateItem(void)
{
	// アイテム発生処理
	UpdateItemGenerator();

	LPITEM pItem = GetItemPtr();

	// 各アイテムの更新処理
	for (int nCntItem = 0; nCntItem < ITEM_CONST::nMaxItem; nCntItem++, pItem++)
	{
		// 当たり判定
		CollisionItem(pItem);

		// 状態更新
		UpdateItemState(pItem);
	}

	// 頂点設定
	SetVertexItem();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawItem(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPITEM pItem = GetItemPtr();	// 弾のポインタ取得

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_itemBuffer.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntItem = 0; nCntItem < ITEM_CONST::nMaxItem; nCntItem++, pItem++)
	{
		if (pItem->obj.bVisible && pItem->bUse)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, g_itemBuffer.apTex[pItem->type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntItem, 2);
		}
	}
}

//=====================================================================
// ポインタ取得処理
//=====================================================================
LPITEM GetItemPtr(void)
{
	return &g_aItem[0];
}

//=====================================================================
// アイテム設置処理
//=====================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, D3DXCOLOR color, D3DXVECTOR2 size)
{
	LPITEM pItem = GetItemPtr();	// アイテムへのポインタ

	for (int nCntItem = 0; nCntItem < ITEM_CONST::nMaxItem; nCntItem++, pItem++)
	{
		if (pItem->bUse) continue;	// 使用済みならスキップ

		// 各情報を保存
		pItem->type = type;							// 種類を保存
		pItem->obj.pos = pos;						// 位置を保存
		pItem->obj.color = color;					// 色を保存
		pItem->obj.rot = D3DXVECTOR3(0, 0, 0);		// 角度を初期化
		pItem->obj.size = ToVector3(size);			// サイズを保存
		pItem->obj.bVisible = true;					// 描画状態を保存
		pItem->state = ITEMSTATE_SPAWN;				// 出現状態
		pItem->nCounterState = ITEM_CONST::aStateCount[ITEMSTATE_SPAWN];	// 状態カウンタを設定

		pItem->bUse = true;							// 使用済みに変更
		break;
	}
}

//=====================================================================
// バッファ作成処理
//=====================================================================
bool CreateItemBuffer(_Out_ ITEMBUFFER *pOut)
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
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (4 * ITEM_CONST::nMaxItem),
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
	for (int nCntTexture = 0; nCntTexture < ITEMTYPE_MAX; nCntTexture++)
	{
		// NULLCHECK
		if (c_apTextureItem[nCntTexture] == nullptr)
		{
			pOut->apTex[nCntTexture] = NULL;
			continue;
		}

		// バッファ作成
		hr = D3DXCreateTextureFromFile(pDevice,
			c_apTextureItem[nCntTexture],
			&pOut->apTex[nCntTexture]);
	}

	return true;
}

//=====================================================================
// 頂点バッファ設定処理
//=====================================================================
void SetVertexItem(void)
{
	VERTEX_2D* pVtx = NULL;				// 頂点情報へのポインタ
	LPITEM pItem = GetItemPtr();		// アイテムのポインタ取得

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_itemBuffer.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < ITEM_CONST::nMaxItem; nCntBullet++, pItem++)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pItem->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pItem->obj.color);
		SetVertexTexturePos(pVtx);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_itemBuffer.pVtxBuff->Unlock();
}

//=====================================================================
// 当たり判定処理
//=====================================================================
void CollisionItem(LPITEM pItem)
{
	// NULLCHECK
	if (pItem == nullptr) return;

	PLAYER* pPlayer = GetPlayer();	// プレイヤーへのポインタ
	if (pPlayer == nullptr) return;	// NULLCHECK

	// アイテムとプレイヤーの位置を判定
	D3DXVECTOR3 length = pPlayer->obj.pos - pItem->obj.pos;
	float fLength = D3DXVec3LengthSq(&length);		// 二点間の距離
	if (fLength <= powf((pItem->obj.size.x * 0.5f) + (pPlayer->obj.size.x * 0.5f), 2.0f))
	{ // 取得判定
		HitItem(pItem, pPlayer);

		SetItemState(pItem, ITEMSTATE_GET);
	}
}

//=====================================================================
// 獲得時の処理
//=====================================================================
void HitItem(LPITEM pItem, PLAYER *pPlayer)
{
	// NULLCHECK
	if (pItem == nullptr || pPlayer == nullptr) return;

	// 種類によって場合分け
	switch (pItem->type)
	{
	case ITEMTYPE_PROTEIN_ALPHA:
	{
		break;
	}

	default:
		break;
	}
}

//=====================================================================
// ステート設定
//=====================================================================
void SetItemState(LPITEM pItem, ITEMSTATE state)		
{
	// NULLCHECK
	if (pItem == nullptr) return;

	// 状態を設定
	pItem->state = state;
	pItem->nCounterState = ITEM_CONST::aStateCount[state];
}

//=====================================================================
// ステートの更新
//=====================================================================
void UpdateItemState(LPITEM pItem)
{
	// NULLCHECK
	if (pItem == nullptr) return;

	// 状態により処理分け
	switch (pItem->state)
	{
	case ITEMSTATE_NONE:
	{
		break;
	}

	case ITEMSTATE_NORMAL:
	{
		pItem->nCounterState--;
		if (pItem->nCounterState == 0)
		{
			// 再度通常状態に設定
			SetItemState(pItem, ITEMSTATE_NORMAL);
		}

		break;
	}

	case ITEMSTATE_SPAWN:
	{
		pItem->nCounterState--;
		if (pItem->nCounterState == 0)
		{
			// 通常状態に設定
			SetItemState(pItem, ITEMSTATE_NORMAL);
		}

		break;
	}

	case ITEMSTATE_GET:
	{
		// アイテムの消去処理
		RemoveItem(pItem);

		// 無に設定
		SetItemState(pItem, ITEMSTATE_NONE);

		break;
	}

	default:
		break;
	}
}

//=====================================================================
// アイテムの消去処理
//=====================================================================
void RemoveItem(LPITEM pItem)	
{
	if (pItem == nullptr) return;	// NULLCHECK

	pItem->bUse = false;
}