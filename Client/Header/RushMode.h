#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)
class CGameObject;
END

class CRushMode
{
public:
	enum class GAME_RESULT { LOSE, WIN, NONE };
private:
	DECLARE_SINGLETON(CRushMode)
private:
	explicit CRushMode();
	virtual ~CRushMode();
public:
	void Initalize(const float& fBossHP, const float& fPlayerHP);
public:
	bool m_bGameEnd;
	bool m_bBossBegin;
	bool m_bBossReady;
	int m_nDeadCount;

	GAME_RESULT m_eResult;

	float m_fBossCurHP;
	float m_fBossMaxHP;

	float m_fPlayerCurHP;
	float m_fPlayerMaxHP;

	int m_nPoint;
};