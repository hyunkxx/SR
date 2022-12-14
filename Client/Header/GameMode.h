#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)
class CGameObject;
END

class CGameMode
{
public:
	enum class TYPE { ALLY, ENEMY, MAX };
	enum class GAME_RESULT { WIN, LOSE, DRAW, NONE };
private:
	DECLARE_SINGLETON(CGameMode)
private:
	explicit CGameMode();
	virtual ~CGameMode();
public:
	void Reset();
	void InitGameMode(_int nVictoryPoint, _float nBaseHP, _int nStartGold);

	bool UseMenu() { return m_bOnSelectButton || m_bOnCreateButton || m_bOnSoundMenu; };
	bool VictoryCheck();

	void Win()
	{
		m_bGameEnd = true;
		m_eGameResult = GAME_RESULT::WIN;
	};
	void Lose()
	{
		m_bGameEnd = true;
		m_eGameResult = GAME_RESULT::LOSE;
	};
public:
	EData eData = {}; //AI 생성용

	bool m_bGameEnd = false;

	bool m_bGameStart = false;
	bool m_bGameExit = false;
	bool m_bWarPressed = false;
	bool m_bRushPressed = false;

	int m_nAllyCount = 0;
	int m_nEnemyCount = 0;
	const int m_nUnitMaxCount = 12;

	int m_nCreateCount = 0;
	int m_nDestroyCount = 0;
	int m_nKillCount = 0;


	bool m_bOnSoundMenu = false;
	bool m_bOnSelectButton = false;
	bool m_bOnCreateButton = false;

	bool m_bOnTrigger = false;

	bool m_bBaseAttack = false;

	GAME_RESULT m_eGameResult;
	_vec3	m_AllyBasePosition = { 0.f, 0.f, 0.f };
	_vec3	m_EnemyBasePosition = { 0.f,0.f,0.f };

	_int	m_nGold[(UINT)TYPE::MAX];		// 재화
	_int	m_nPoint[(UINT)TYPE::MAX];		// 승점 포인트
	_int	m_nVictoryPoint = 0;

	_float	m_fBaseCurHP[(UINT)TYPE::MAX];	// 기지 현재체력
	_float	m_fBaseMaxHP[(UINT)TYPE::MAX];	// 기지 최대체력
};