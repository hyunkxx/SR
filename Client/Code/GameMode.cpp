#include "stdafx.h"
#include "..\Header\GameMode.h"

#include "Export_Function.h"

#include "Humvee.h"
#include "SmallTank.h"
#include "MiddleTank.h"
#include "BigTank.h"
#include "LongTank.h"

IMPLEMENT_SINGLETON(CGameMode)

CGameMode::CGameMode()
{
	ZeroMemory(&m_nGold, sizeof(m_nGold));
	ZeroMemory(&m_nPoint, sizeof(m_nPoint));
	ZeroMemory(&m_nVictoryPoint, sizeof(m_nVictoryPoint));
	ZeroMemory(&m_fBaseCurHP, sizeof(m_fBaseCurHP));
	ZeroMemory(&m_fBaseMaxHP, sizeof(m_fBaseMaxHP));
}


CGameMode::~CGameMode()
{
}

void CGameMode::Free(void)
{
}

void CGameMode::InitGameMode(_int nVictoryPoint, _float nBaseHP, _int nStartGold)
{
	m_nVictoryPoint = nVictoryPoint;
	
	m_nPoint[(UINT)TYPE::ALLY] = 0;
	m_nPoint[(UINT)TYPE::ENEMY] = 0;

	m_fBaseMaxHP[(UINT)TYPE::ALLY] = nBaseHP;
	m_fBaseMaxHP[(UINT)TYPE::ENEMY] = nBaseHP;
	memcpy(&m_fBaseCurHP, m_fBaseMaxHP, sizeof(m_fBaseCurHP));

	m_nGold[(UINT)TYPE::ALLY] = nStartGold;
	m_nGold[(UINT)TYPE::ENEMY] = nStartGold;
}

bool CGameMode::VictoryCheck()
{
	if (m_nPoint[(UINT)TYPE::ALLY] < m_nVictoryPoint
		&& m_nPoint[(UINT)TYPE::ENEMY] < m_nVictoryPoint)
		return false;
	
	if (m_nPoint[(UINT)TYPE::ALLY] > m_nPoint[(UINT)TYPE::ENEMY])
	{
		m_eGameResult = GAME_RESULT::WIN;
	}
	else if (m_nPoint[(UINT)TYPE::ALLY] < m_nPoint[(UINT)TYPE::ENEMY])
	{
		m_eGameResult = GAME_RESULT::LOSE;
	}
	else
	{
		m_eGameResult = GAME_RESULT::DRAW;
	}
	return true;
}

void CGameMode::Reset()
{
	m_eGameResult = GAME_RESULT::NONE;
	ZeroMemory(&m_nGold, sizeof(m_nGold));
	ZeroMemory(&m_nPoint, sizeof(m_nPoint));

	m_fBaseCurHP[(UINT)TYPE::ALLY] = m_fBaseMaxHP[(UINT)TYPE::ALLY];
	m_fBaseCurHP[(UINT)TYPE::ENEMY] = m_fBaseMaxHP[(UINT)TYPE::ENEMY];
}
