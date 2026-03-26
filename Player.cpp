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
#include "Camera.h"
#include "Enemy.h"
#include "LevelGenerator.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "particle.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\PlayerAnimationGORI.png"
#define NUM_TEXTURE_X		(4)
#define NUM_TEXTURE_Y		(2)
#define INIT_POS			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(75.0f, 75.0f, 75.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE/*D3DXCOLOR(1.0, 0.847, 0.58, 1.0)*/

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	PLAYER_CONTROLTYPE_MOUSE = 0,
	PLAYER_CONTROLTYPE_JOYSTICK,
	PLAYER_CONTROLTYPE_MAX
}PLAYER_CONTROLTYPE;

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
void _PlayerControl();
void _OnPlayerState();

ENEMY* _GetNearestEnemy(void);
void _AttackNearestEnemy(void);
void _OnEnemyEnteredAttackRange(ENEMY* pEnemy);
void _OnEnemyKilled(ENEMY* pEnemy);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_Player;

// プレイヤー操作タイプ
// これによって入力方法と方向の求め方が変わる
PLAYER_CONTROLTYPE g_ctrlType = PLAYER_CONTROLTYPE_MOUSE;

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
	g_Player.move = g_Player.obj.pos + D3DXVECTOR3(0, 1, 0);
	g_Player.nMaxBullet = PLAYER_MAX_HOLDABLE_BULLET;
	g_Player.fBulletSpeed = PLAYER_INIT_BULLETSPEED;
	g_Player.nBulletLeft = PLAYER_MAX_HOLDABLE_BULLET;
	g_Player.fSpeed = PLAYER_INIT_SPEED;
	g_Player.nPower = PLAYER_INIT_POWER;
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.bControlEnabled = true;

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

	GeneratorLevel(
		g_Player.nIdxLevel,
		g_Player.obj.pos
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
	_PlayerControl();

	_OnPlayerState();
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
	int nTextureX = g_Player.nTexture % NUM_TEXTURE_X;
	int nTextureY = g_Player.nTexture / NUM_TEXTURE_X;
	SetVertexTexturePos(
		pVtx,
		nTextureX,
		nTextureY,
		NUM_TEXTURE_X,
		NUM_TEXTURE_Y, 
		g_Player.obj.bInversed
	);

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

void SetPlayerState(PLAYERSTATE newState)
{
	g_Player.state = newState;
	g_Player.nCounterState = 0;
}

PLAYERSTATE GetPlayerState(void)
{
	return g_Player.state;
}

void SmashPlayer(D3DXVECTOR3 dir)
{
	g_Player.bControlEnabled = false;
	g_Player.move = dir * 10.0f;
	ShakeCamera(30);
	SetPlayerState(PLAYERSTATE_SMASH);
	PlaySound(SOUND_LABEL_SE_PLAYERSMASH);
}

void AddScore(int nValue)
{
	g_Player.nScore += nValue;
}

void _PlayerControl()
{
	if (g_Player.bControlEnabled == false) return;

	DIMOUSESTATE mouse = GetMouse();
	XINPUT_STATE* joypad = GetJoypad();
	XINPUT_GAMEPAD gamepad = joypad->Gamepad;
	D3DXVECTOR2 vecThumbL = GetJoystickThumbLMagnitude();
	D3DXVECTOR2 vecMouseMove = D3DXVECTOR2(mouse.lX, mouse.lY);
	D3DXVECTOR3 direction = D3DXVECTOR3_ZERO;

	// 直前にマウスとコントローラーどちらの入力があったか確認し
	// それに応じて方向ベクトルの算出方法を変える
	if (g_ctrlType == PLAYER_CONTROLTYPE_MOUSE)
	{// マウス
		// マウス位置はスクリーン座標上にあるのでスクリーン中心値からマウス位置までの
		// 方向ベクトルを移動先にする
		direction = Normalize(Vector2To3(GetMousePos()) - D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0));

		// コントローラーの入力があった場合入力方法を変える
		if (fabsf(vecThumbL.x + vecThumbL.y) > 0)
		{
			g_ctrlType = PLAYER_CONTROLTYPE_JOYSTICK;
		}
	}
	else if (g_ctrlType == PLAYER_CONTROLTYPE_JOYSTICK)
	{// ジョイスティック
		direction = Normalize(D3DXVECTOR3(vecThumbL.x, -vecThumbL.y, 0));

		// マウスの入力があった場合入力方法を変える
		if (fabsf(vecMouseMove.x + vecMouseMove.y) > 0)
		{
			g_ctrlType = PLAYER_CONTROLTYPE_MOUSE;
		}
	}

	// 入力方向が変わった時だけ移動方向も変える
	if (fabsf(direction.x + direction.y) > 0)
	{
		g_Player.move = direction;
	}

	float fRotDest = atan2f(g_Player.move.x, g_Player.move.y);

	// プレイヤーをマウス方向に傾ける（角度値は-pi~piに補正）
	g_Player.obj.rot.z += (GetFixedRotation(fRotDest - g_Player.obj.rot.z)) * PLAYER_INIT_ROTSPEED;
	g_Player.obj.rot.z = GetFixedRotation(g_Player.obj.rot.z);

	g_Player.obj.pos += Direction(g_Player.obj.rot.z) * g_Player.fSpeed;
}

void _OnPlayerState()
{
	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
	{
		// カメラをプレイヤーに追従させる
		// 若干向いてる方向にオフセットする
		float fCamOffsetMagnitude = 100.0f;
		D3DXVECTOR3 vecCamOffset = Direction(g_Player.obj.rot.z) * fCamOffsetMagnitude;
		GetCamera()->pos = g_Player.obj.pos + vecCamOffset;

		// レベル表示をプレイヤーの頭上に追従させる
		D3DXVECTOR3 vecLevelOffset = D3DXVECTOR3(-50, -g_Player.obj.size.y, 0);
		SetPositionLevel(
			g_Player.nIdxLevel,
			g_Player.obj.pos + vecLevelOffset
		);

		// テクスチャアニメーション（現在のテクスチャ位置を更新）
		g_Player.nTexture = (g_Player.nTexture + 1) % (NUM_TEXTURE_X * NUM_TEXTURE_Y);

		// 一番近くの敵１人だけを攻撃
		_AttackNearestEnemy();
		break;
	}

	case PLAYERSTATE_SUPER:
	{
		// カメラをプレイヤーに追従させる
		// 若干向いてる方向にオフセットする
		float fCamOffsetMagnitude = 100.0f;
		D3DXVECTOR3 vecCamOffset = Direction(g_Player.obj.rot.z) * fCamOffsetMagnitude;
		GetCamera()->pos = g_Player.obj.pos + vecCamOffset;

		// レベル表示をプレイヤーの頭上に追従させる
		D3DXVECTOR3 vecLevelOffset = D3DXVECTOR3(-50, -g_Player.obj.size.y, 0);
		SetPositionLevel(
			g_Player.nIdxLevel,
			g_Player.obj.pos + vecLevelOffset
		);

		// テクスチャアニメーション（現在のテクスチャ位置を更新）
		g_Player.nTexture = (g_Player.nTexture + 1) % (NUM_TEXTURE_X * NUM_TEXTURE_Y);

		// 一番近くの敵１人だけを攻撃
		_AttackNearestEnemy();
		break;
	}
	
	case PLAYERSTATE_SMASH:
	{
		// SmashPlayer()で設定されたベクトルに向かって回転しながら吹き飛ぶ
		g_Player.obj.pos += g_Player.move;
		g_Player.obj.rot.z += 0.5f;

		if (g_Player.nCounterState > 90)
		{
			SetFade(SCENE_RESULT);
		}

		EFFECTINFO info;
		info.col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
		info.fMaxAlpha = 0.1f;
		info.fMaxScale = 0.7f;
		info.fRotSpeed = 0.1f;
		info.fSpeed = 0.5f;
		info.nMaxLife = 120;

		SetParticle(info, g_Player.obj.pos, 0, D3DX_PI, 1, 1);

		// レベル表示をプレイヤーの頭上に追従させる
		D3DXVECTOR3 vecLevelOffset = D3DXVECTOR3(-50, -g_Player.obj.size.y, 0);
		SetPositionLevel(
			g_Player.nIdxLevel,
			g_Player.obj.pos + vecLevelOffset
		);
		break;
	}

	default:
		break;
	}

	g_Player.nCounterState++;
}

ENEMY* _GetNearestEnemy(void)
{
	ENEMY* pEnemy = GetEnemy();
	ENEMY* pNearestEnemy = pEnemy;
	float fMaxDistance = 100000.0f;

	for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
	{
		if (pEnemy->bUsed == false) continue;

		float fDistance = Magnitude(g_Player.obj.pos, pEnemy->obj.pos);

		if (fDistance < fMaxDistance)
		{
			fMaxDistance = fDistance;
			pNearestEnemy = pEnemy;
		}
	}

	return pNearestEnemy;
}

void _AttackNearestEnemy(void)
{
	ENEMY* pEnemy = _GetNearestEnemy();

	if (pEnemy == NULL) return;
	if (pEnemy->bUsed == false) return;

	float fAttackRange = (pEnemy->obj.size.x * 0.5f + pEnemy->obj.size.x * 0.5f) * 2;

	if (Magnitude(pEnemy->obj.pos, g_Player.obj.pos) < fAttackRange)
	{
		_OnEnemyEnteredAttackRange(pEnemy);
	}
}

void _OnEnemyEnteredAttackRange(ENEMY* pEnemy)
{
	bool bHasEnemyDied = DamageEnemy(pEnemy, g_Player.nPower);

	if (bHasEnemyDied)
	{
		_OnEnemyKilled(pEnemy);
	}
	else
	{
		// ノックバック（プレイヤーに向かって反対に方向に押し出す）
		pEnemy->obj.pos += Direction(g_Player.obj.pos, pEnemy->obj.pos) * g_Player.fSpeed * 0.75f;

		// 若干揺らす
		pEnemy->obj.pos += Vector2To3(GetRandomVector2()) * pEnemy->obj.size.x * 0.05f;
	}

	// パンチエフェクトの生成
	float fPunchOffset = g_Player.obj.size.x;
	D3DXVECTOR3 vecPlrToEnemy = Direction(g_Player.obj.pos, pEnemy->obj.pos);
	D3DXVECTOR3 punchStart = g_Player.obj.pos + (vecPlrToEnemy * fPunchOffset) + Vector2To3(GetRandomVector2() * 100);
	D3DXVECTOR3 punchDir = Direction(punchStart, pEnemy->obj.pos);
	D3DXVECTOR3 punchRot = D3DXVECTOR3(0, 0, atan2f(punchDir.x, punchDir.y));
	GenerateBullet(punchStart, punchRot, 10, 0, 10, BT_TEST);

	// 効果音
	switch (rand() % 3)
	{
	case 0:
		PlaySound(SOUND_LABEL_SE_PUNCH_00);
		break;

	case 1:
		PlaySound(SOUND_LABEL_SE_PUNCH_01);
		break;

	case 2:
		PlaySound(SOUND_LABEL_SE_PUNCH_02);
		break;

	default:
		break;
	}
}

void _OnEnemyKilled(ENEMY* pEnemy)
{
	ShakeCamera(10);
	AddLevel(g_Player.nIdxLevel, 1);
	PlaySound(SOUND_LABEL_SE_SMASH);
}