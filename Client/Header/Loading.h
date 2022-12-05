#pragma once
#include "Base.h"
#include "Include.h"

class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	LOADINGID				Get_LoadingID(void) const { return m_eID; }
	CRITICAL_SECTION*		Get_Crt(void)     { return &m_Crt; }
	_bool					Get_Finish(void) const { return m_bFinish; }
	const _tchar*			Get_String(void) { return m_szString; }

public:
	HRESULT			Ready_Loading(LOADINGID eID);
	_uint			Loading_ForStage(void);

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

private:
	virtual		void		Collison_Object(void) {};

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eID;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;
	_tchar				m_szString[128];

public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
	virtual void			Free(void);

};

