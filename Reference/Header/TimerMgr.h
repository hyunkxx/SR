#pragma once

#include "Base.h"
#include "Timer.h"
#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

public:
	_float		Get_Timer(const _tchar* pTimerTag);
	void		Set_Timer(const _tchar* pTimerTag);

public:
	HRESULT		Ready_Timer(const _tchar* pTimerTag);
	
private:
	CTimer*		Find_Timer(const _tchar* pTimerTag);
	
private:
	unordered_map<const _tchar*, CTimer*>		m_umapTimers;

public:
	virtual	void Free(void)override;
};

END
