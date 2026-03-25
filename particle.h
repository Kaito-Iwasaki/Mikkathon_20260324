//=====================================================================
// 
// particle.cppのヘッダファイル [particle.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_		// 二重インクルード防止のマクロ

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"
#include "effect.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_PARTICLE		(256)

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
typedef struct
{
	BASEOBJECT obj;
	bool bUsed;
	EFFECTINFO info;
	float fAngle;
	float fRange;
	int nLife;
	int nNumEffect;
	D3DXVECTOR3* pPosParent;
}PARTICLE;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void SetParticle(EFFECTINFO info, D3DXVECTOR3 pos, float fAngle, float fRange, int nLife, int nNumEffect);
void SetParticle(EFFECTINFO info, D3DXVECTOR3* pPos, float fAngle, float fRange, int nLife, int nNumEffect);

#endif