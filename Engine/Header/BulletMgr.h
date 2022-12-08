#pragma once
#include "Base.h"
#include "Include.h"



BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CBulletMgr :
	public CBase
{
	DECLARE_SINGLETON(CBulletMgr)
private:
	explicit CBulletMgr();
	virtual ~CBulletMgr();

public:
	void Update_BulletMgr(const _float & fTimeDelta);
	void LateUpdate_BulletMgr(void);
public:
	void Bullet_Supply(CGameObject* pBullet, BULLET_ID eID);
	void Collect_Object(void);
	void Reuse_Object(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float fAngleY, BULLET_ID eID);
	list<CGameObject*>* Get_Bullet_List(BULLET_ID eID) { return &m_vBullet_War[eID]; }
private:

	queue<CGameObject*> m_qBullet_Ammunition_Depot[BULLET_ID::BULLET_END];
	// 전장에 나돌아 다니는 탄약
	list<CGameObject*> m_vBullet_War[BULLET_ID::BULLET_END];
public:
	virtual void Free(void)override;
};

END
