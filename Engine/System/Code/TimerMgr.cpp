#include "..\..\Header\TimerMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

CTimerMgr::CTimerMgr()
{
}

CTimerMgr::~CTimerMgr()
{
	Free();
}

_float Engine::CTimerMgr::Get_Timer(const _tchar* pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);
	NULL_CHECK_RETURN(pTimer, 0.f);

	return pTimer->Get_TimeDelta();
}

void CTimerMgr::Set_Timer(const _tchar * pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);
	NULL_CHECK(pTimer);

	pTimer->Update_Timer();
}

CTimer* Engine::CTimerMgr::Find_Timer(const _tchar* pTimerTag)
{
	auto		iter = find_if(m_umapTimers.begin(), m_umapTimers.end(), CTag_Finder(pTimerTag));

	if (iter == m_umapTimers.end())
		return nullptr;

	return iter->second;
}

HRESULT Engine::CTimerMgr::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);
	
	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	m_umapTimers.insert({ pTimerTag, pTimer });

	return S_OK;
}

void Engine::CTimerMgr::Free(void)
{
	for_each(m_umapTimers.begin(), m_umapTimers.end(), CDeleteMap());
	m_umapTimers.clear();
}
