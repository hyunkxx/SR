#include "..\..\Header\SP_EffectMgr.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CSP_EffectMgr);

CSP_EffectMgr::CSP_EffectMgr()
{
}


CSP_EffectMgr::~CSP_EffectMgr()
{
	Free();
}

void CSP_EffectMgr::Update_CSP_EffectMgr(const _float & fTimeDelta)
{
	for (_int i = 0; EFFECT_ID::EFFECT_END > i; i++)
	{
		for (auto& iter : m_vEffect_War[i])
			iter->Update_Object(fTimeDelta);
	}
}

void CSP_EffectMgr::LateUpdate_CSP_EffectMgr(void)
{
	for (_int i = 0; EFFECT_ID::EFFECT_END > i; i++)
	{
		for (auto& iter : m_vEffect_War[i])
			iter->LateUpdate_Object();
	}

	Collect_Effect();
}

void CSP_EffectMgr::Effect_Supply(CGameObject * pBullet, EFFECT_ID eID)
{
	if (pBullet == nullptr && 0 > eID && EFFECT_END <= eID)
		return;

	// 여기서 필요하다면 아이디 셋팅

	m_qEffect_Depot[eID].push(pBullet);
}

void CSP_EffectMgr::Collect_Effect(void)
{
	for (_int i = 0; EFFECT_ID::EFFECT_END > i; i++)
	{
		for (auto& iter = m_vEffect_War[i].begin(); iter != m_vEffect_War[i].end();)
		{
			if ((*iter)->Get_Dead())
			{
				// 여기서 리셋 후 넣기

				m_qEffect_Depot[i].push(*iter);
				iter = m_vEffect_War[i].erase(iter);
			}
			else
				iter++;
		}
	}
}

CGameObject* CSP_EffectMgr::Reuse_Effect(EFFECT_ID eID)
{
	CGameObject* pEffect = m_qEffect_Depot[eID].front();

	// 여기서 필요하다면 셋팅
	m_vEffect_War[eID].push_back(pEffect);
	m_qEffect_Depot[eID].pop();

	return pEffect;
}

void CSP_EffectMgr::Free(void)
{
	for (_int i = 0; EFFECT_ID::EFFECT_END > i; i++)
	{
		while (!m_qEffect_Depot[i].empty())
		{
			CGameObject* pEffect = m_qEffect_Depot[i].front();
			pEffect->Free();
			Safe_Delete(pEffect);
			m_qEffect_Depot[i].pop();
		}
	}

	for (_int i = 0; EFFECT_ID::EFFECT_END > i; i++)
	{
		for (auto&iter = m_vEffect_War[i].begin(); iter != m_vEffect_War[i].end();)
		{
			(*iter)->Free();
			Safe_Delete((*iter));
			iter = m_vEffect_War[i].erase(iter);
		}
		m_vEffect_War[i].clear();
	}
}
