//=====================================================================
//
// Player [Player.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Player.h"
#include "input.h"
#include "bulletGenerator.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	NULL
#define INIT_POS			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(50.0f, 50.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_Player;

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_Player, 0, sizeof(PLAYER));
	g_Player.obj.pos = INIT_POS;
	g_Player.obj.size = INIT_SIZE;
	g_Player.obj.color = INIT_COLOR;
	g_Player.obj.bVisible = true;
	g_Player.nMaxBullet = PLAYER_MAX_HOLDABLE_BULLET;
	g_Player.fBulletSpeed = PLAYER_BULLETSPEED;
	g_Player.nBulletLeft = PLAYER_MAX_HOLDABLE_BULLET;

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffPlayer
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPlayer(void)
{
	if (g_pTexBuffPlayer != NULL)
	{// テクスチャの破棄
		g_pTexBuffPlayer->Release();
		g_pTexBuffPlayer = NULL;
	}

	if (g_pVtxBuffPlayer != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePlayer(void)
{
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;

	// マウス位置を目的地に設定
	direction = Vector2To3(GetMousePos());
	g_Player.move = Direction(g_Player.obj.pos, direction);

	float fRotDest = atan2f(g_Player.move.x, g_Player.move.y);

	// プレイヤーをマウス方向に傾ける（角度値は-pi~piに補正）
	g_Player.obj.rot.z += (GetFixedRotation(fRotDest - g_Player.obj.rot.z)) * PLAYER_ROTSPEED;
	g_Player.obj.rot.z = GetFixedRotation(g_Player.obj.rot.z);

	g_Player.obj.pos += Direction(g_Player.obj.rot.z) * PLAYER_SPEED;

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		GenerateBullet(g_Player.obj.pos, g_Player.obj.rot, 5, 0.1f, BT_TEST);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_Player.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_Player.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Player.obj.bVisible)
	{// 表示状態
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffPlayer);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// プレイヤー取得処理
//=====================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player;
}