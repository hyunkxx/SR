#pragma once

#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL CKeyMgr : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	CKeyMgr();
	~CKeyMgr();
public:
	HRESULT Ready_KeyMgr(void);
	void Update_KeyMgr(void);
	KEY_STATE Get_KeyState(int _key) { return vecKeyList[(int)_key].eKeyState; }

private:
	tagKey vecKeyList[VK_MAX];
	static CKeyMgr* m_pKeyMgr;

public:
	virtual void Free(void)override;
};

END
