#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CEnermyMgr :public CBase
{
	DECLARE_SINGLETON(CEnermyMgr)
private:
	explicit CEnermyMgr();
	virtual ~CEnermyMgr();

public:
	void Enermy_Add(CGameObject* pBullet, OBJID eID);
	void CreateObject(OBJID eID);

public:
	void Update_EnermyMgr(const _float & fTimeDelta);
	void LateUpdate_EnermyMgr(void);
	vector<CGameObject*> Get_mIEnermy(OBJID eID) { return m_lEnermy[eID]; }
private:
	vector<CGameObject*> m_lEnermy[OBJID::OBJID_END];
public:
	virtual void Free(void)override;
};

END