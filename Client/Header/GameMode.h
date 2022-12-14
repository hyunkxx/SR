#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)
class CGameObject;
END

class CGameMode : public CBase
{
public:
	enum class TYPE { ALLY, ENEMY, MAX };
	enum class GAME_RESULT { WIN, LOSE, DRAW, NONE };
private:
	DECLARE_SINGLETON(CGameMode)
private:
	explicit CGameMode();
	virtual ~CGameMode();
	inline virtual void	Free(void);
public:
	void Reset();
	void InitGameMode(_int nVictoryPoint, _float nBaseHP, _int nStartGold);

	bool VictoryCheck();
public:
	GAME_RESULT m_eGameResult;

	_int	m_nGold[(UINT)TYPE::MAX];		// 재화
	_int	m_nPoint[(UINT)TYPE::MAX];		// 승점 포인트
	_int	m_nVictoryPoint;

	_float	m_fBaseCurHP[(UINT)TYPE::MAX];	// 기지 현재체력
	_float	m_fBaseMaxHP[(UINT)TYPE::MAX];	// 기지 최대체력
};