#include "..\..\Header\BulletMgr.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CBulletMgr)

CBulletMgr::CBulletMgr()
{
}


CBulletMgr::~CBulletMgr()
{
	Free();
}

void CBulletMgr::Update_BulletMgr(const _float & fTimeDelta)
{
	for (_int i = 0; BULLET_ID::BULLET_END > i; i++)
	{
		for (auto& iter : m_vBullet_War[i])
			iter->Update_Object(fTimeDelta);
	}
}

void CBulletMgr::LateUpdate_BulletMgr(void)
{

	for (_int i = 0; BULLET_ID::BULLET_END > i; i++)
	{
		for (auto& iter : m_vBullet_War[i])
			iter->LateUpdate_Object();
	}

	Collect_Object();
}

void CBulletMgr::Bullet_Supply(CGameObject * pBullet, BULLET_ID eID)
{
	if (pBullet == nullptr && 0 > eID && BULLET_END <= eID)
		return;

	if (dynamic_cast<CBullet*>(pBullet))
		static_cast<CBullet*>(pBullet)->Set_ID(eID);
	else if (dynamic_cast<CSmoke_Bullet*>(pBullet))
		static_cast<CSmoke_Bullet*>(pBullet)->Set_ID(eID);
	else if (dynamic_cast<CBoom_Bullet*>(pBullet))
		static_cast<CBoom_Bullet*>(pBullet)->Set_ID(eID);

	m_qBullet_Ammunition_Depot[eID].push(pBullet);
}

void CBulletMgr::Collect_Object(void)
{
	for (_int i = 0; BULLET_ID::BULLET_END > i; i++)
	{
		for (auto& iter = m_vBullet_War[i].begin(); iter != m_vBullet_War[i].end();)
		{
			if ((*iter)->Get_Dead())
			{
				if (dynamic_cast<CBullet*>(*iter))
					static_cast<CBullet*>(*iter)->Reset_Trans();
				else if (dynamic_cast<CSmoke_Bullet*>(*iter))
					static_cast<CSmoke_Bullet*>(*iter)->Reset_Trans();
				else if (dynamic_cast<CBoom_Bullet*>(*iter))
					static_cast<CBoom_Bullet*>(*iter)->Reset_Trans();

				m_qBullet_Ammunition_Depot[i].push(*iter);
				iter = m_vBullet_War[i].erase(iter);
			}
			else
				iter++;
		}
	}
}

CGameObject* CBulletMgr::Reuse_Object(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float fAngleY, BULLET_ID eID)
{

	if (0 > eID ||  BULLET_END <= eID || !m_qBullet_Ammunition_Depot[eID].size())
		return nullptr;

	CGameObject* pBullet = m_qBullet_Ammunition_Depot[eID].front();
	

	if (dynamic_cast<CBullet*>(pBullet))
		static_cast<CBullet*>(pBullet)->Bullet_Setting(vPos, vDir, fSpeed, fAngleX, fAngleY);
	else if (dynamic_cast<CSmoke_Bullet*>(pBullet))
		static_cast<CSmoke_Bullet*>(pBullet)->Bullet_Setting(vPos, vDir, fSpeed, fAngleX, fAngleY);
	else if (dynamic_cast<CBoom_Bullet*>(pBullet))
		static_cast<CBoom_Bullet*>(pBullet)->Bullet_Setting(vPos, vDir, fSpeed, fAngleX, fAngleY);

	m_vBullet_War[eID].push_back(pBullet);
	m_qBullet_Ammunition_Depot[eID].pop();

	return pBullet;
}

void CBulletMgr::Free(void)
{

	for (_int i = 0; BULLET_ID::BULLET_END > i; i++)
	{
		while (!m_qBullet_Ammunition_Depot[i].empty())
		{
			CGameObject* pBullet = m_qBullet_Ammunition_Depot[i].front();
			Safe_Delete(pBullet);
			m_qBullet_Ammunition_Depot[i].pop();
		}
	}

	for (_int i = 0; BULLET_ID::BULLET_END > i; i++)
	{
		for (auto&iter = m_vBullet_War[i].begin(); iter != m_vBullet_War[i].end();)
		{
			Safe_Delete((*iter));
			iter = m_vBullet_War[i].erase(iter);
		}
		m_vBullet_War[i].clear();
	}
}
