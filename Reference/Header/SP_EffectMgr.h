#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CSP_EffectMgr :
	public CBase
{
	DECLARE_SINGLETON(CSP_EffectMgr)
private:
	CSP_EffectMgr();
	virtual ~CSP_EffectMgr();
public:
	void Update_CSP_EffectMgr(const _float & fTimeDelta);
	void LateUpdate_CSP_EffectMgr(void);
public:
	void Effect_Supply(CGameObject* pBullet, EFFECT_ID eID);
	void Collect_Effect(void);
	CGameObject* Reuse_Effect(EFFECT_ID eID);

private:
	queue<CGameObject*> m_qEffect_Depot[EFFECT_ID::EFFECT_END];
	// 전장에 나돌아 다니는 이펙트
	list<CGameObject*> m_vEffect_War[EFFECT_ID::EFFECT_END];
public:
	virtual void Free(void)override;
};

END
