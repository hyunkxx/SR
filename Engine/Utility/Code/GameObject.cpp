#include "..\..\Header\GameObject.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bDead(false)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev) , m_bDead(rhs.m_bDead)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);

	if (nullptr == pComponent)
		return nullptr;
	
	return pComponent;
}

void CGameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

	_vec3 vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

HRESULT CGameObject::Ready_Object(void)
{
	Set_CollisionDist();
	return S_OK;
}

_int CGameObject::Update_Object(const _float & fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);
	
	return 0;
}

void CGameObject::LateUpdate_Object(void)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();
}

void CGameObject::Render_Object(void)
{
}

void CGameObject::RenderGUI(void)
{
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;
	
	return iter->second;
}

void CGameObject::Set_CollisionDist(void)
{
	m_fCollisionDist  = sqrtf((m_stBody.fLen[y] * m_stBody.fLen[y]) + (sqrtf((m_stBody.fLen[x] * m_stBody.fLen[x]) + (m_stBody.fLen[z] * m_stBody.fLen[z])) * sqrtf((m_stBody.fLen[x] * m_stBody.fLen[x]) + (m_stBody.fLen[z] * m_stBody.fLen[z]))));
}

void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}
