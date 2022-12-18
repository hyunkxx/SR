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
	:m_bGameEnd(false)
	, m_nCreateCount(0)
	, m_nDestroyCount(0)
	, m_nKillCount(0)
{
	ZeroMemory(&m_nGold, sizeof(m_nGold));
	ZeroMemory(&m_nPoint, sizeof(m_nPoint));
	ZeroMemory(&m_nVictoryPoint, sizeof(m_nVictoryPoint));
	ZeroMemory(&m_fBaseCurHP, sizeof(m_fBaseCurHP));
	ZeroMemory(&m_fBaseMaxHP, sizeof(m_fBaseMaxHP));
	ZeroMemory(&eData, sizeof(EData));
}

CGameMode::~CGameMode()
{

}

void CGameMode::InitGameMode(_int nVictoryPoint, _float nBaseHP, _int nStartGold)
{
	ZeroMemory(&m_nGold, sizeof(m_nGold));
	ZeroMemory(&m_nPoint, sizeof(m_nPoint));
	ZeroMemory(&m_nVictoryPoint, sizeof(m_nVictoryPoint));
	ZeroMemory(&m_fBaseCurHP, sizeof(m_fBaseCurHP));
	ZeroMemory(&m_fBaseMaxHP, sizeof(m_fBaseMaxHP));
	ZeroMemory(&eData, sizeof(EData));
	m_bGameStart = false;
	m_bGameExit = false;
	m_bWarPressed = false;
	m_bRushPressed = false;

	m_nAllyCount = 0;
	m_nEnemyCount = 0;
	m_bOnSoundMenu = false;
	m_bOnSelectButton = false;
	m_bOnCreateButton = false;
	m_bGameEnd = false;
	m_bOnTrigger = false;
	m_nCreateCount = 0;
	m_nDestroyCount = 0;
	m_nKillCount = 0;
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
