#include "..\..\Header\EnermyMgr.h"

#include "Export_Function.h"
USING(Engine)
IMPLEMENT_SINGLETON(CEnermyMgr)
CEnermyMgr::CEnermyMgr()
{
}


CEnermyMgr::~CEnermyMgr()
{
	Free();
}

void CEnermyMgr::Enermy_Add(CGameObject * pEnermy, OBJID eID)
{
	if (pEnermy == nullptr && 0 > eID && OBJID_END <= eID)
		return;

	m_lEnermy[eID].push_back(pEnermy);
}

void CEnermyMgr::CreateObject(OBJID eID)
{
}

void CEnermyMgr::Update_EnermyMgr(const _float & fTimeDelta)
{
	for (_int i = 0; OBJID::OBJID_END > i; i++)
	{
		for (auto& iter : m_lEnermy[i])
			iter->Update_Object(fTimeDelta);
	}
}

void CEnermyMgr::LateUpdate_EnermyMgr(void)
{
	for (_int i = 0; OBJID::OBJID_END > i; i++)
	{
		for (auto& iter : m_lEnermy[i])
			iter->LateUpdate_Object();
	}
}

void CEnermyMgr::Free(void)
{



	for (_int i = 0; BULLET_ID::BULLET_END > i; i++)
	{
		for (auto&iter = m_lEnermy[i].begin(); iter != m_lEnermy[i].end();)
		{
			Safe_Delete((*iter));
			iter = m_lEnermy[i].erase(iter);
		}
		m_lEnermy[i].clear();
	}
}
