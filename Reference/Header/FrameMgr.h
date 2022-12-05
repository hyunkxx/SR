#pragma once

#include "Base.h"
#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrameMgr : public CBase
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	explicit CFrameMgr();
	virtual ~CFrameMgr();

public:
	HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fTimeDelta);
	_bool		IsPermit_Call(const _tchar * pFrameTag, const _float& fTimeDelta);

private:
	CFrame*		Find_Frame(const _tchar* pFrameTag);

private:
	unordered_map<const _tchar*, CFrame*>		m_umapFrame;

public:
	virtual	void Free(void)override;
};

END
