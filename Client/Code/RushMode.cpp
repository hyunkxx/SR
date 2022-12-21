#include "stdafx.h"
#include "..\Header\RushMode.h"

#include "Export_Function.h"

#include "Humvee.h"
#include "SmallTank.h"
#include "MiddleTank.h"
#include "BigTank.h"
#include "LongTank.h"

IMPLEMENT_SINGLETON(CRushMode)

CRushMode::CRushMode()
	: m_bGameEnd(false)
	, m_eResult(GAME_RESULT::NONE)
	, m_nPoint(500)
{

}

CRushMode::~CRushMode()
{

}

void CRushMode::Initalize(const float & fBossHP, const float & fPlayerHP)
{
	m_bGameEnd = false;
	m_eResult = GAME_RESULT::NONE;

	m_fBossCurHP = fBossHP;
	m_fBossMaxHP = fBossHP;

	m_fPlayerCurHP = fPlayerHP;
	m_fPlayerMaxHP = fPlayerHP;

	m_nPoint = 500;
}
