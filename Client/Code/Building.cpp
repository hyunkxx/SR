#include "stdafx.h"
#include "..\Header\Building.h"

#include "Utility.h"
#include "Export_Function.h"
#include "Component.h"
#include "Transform.h"
#include "Voxel.h"

USING(Engine)

_int CBuilding::ID = -1;

CBuilding* CBuilding::Create(LPDIRECT3DDEVICE9 pGramphicDev, const _vec3& vPos)
{
	ID++;
	CBuilding* pInstance = new CBuilding(pGramphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Building::Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

//일단 반복문 자체에서 갯수안맞음 // 그리고 컬러 셋컬러수정 전체 복셀의 커러가 바뀜
void CBuilding::Free(void)
{
	__super::Free();
}

CBuilding::~CBuilding()
{
}

CBuilding::CBuilding(LPDIRECT3DDEVICE9 pGramphicDev, const _vec3& vPos)
	: CGameObject(pGramphicDev)
	, m_vPosition(vPos)
{
}

CBuilding::CBuilding(const CBuilding& rhs)
	: CGameObject(rhs)
	, m_vPosition(rhs.m_vPosition)
{
}

HRESULT CBuilding::Ready_Object(void)
{
	m_ID = L"building" + to_wstring(ID);
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	D3DXMatrixIdentity(&m_matWorld);
	_matrix matTrans;

	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matTrans;

	return S_OK;
}

_int CBuilding::Update_Object(const _float & fTimeDelta)
{
	if (Utility::Cuilling(m_pGraphicDev, m_vPosition))
		return 0;

	__super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CBuilding::LateUpdate_Object(void)
{
	if (Utility::Cuilling(m_pGraphicDev, m_vPosition))
		return;

	__super::LateUpdate_Object();
}

void CBuilding::Render_Object(void)
{
	if (Utility::Cuilling(m_pGraphicDev, m_vPosition))
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pMesh->Render(&m_matWorld);
}

void CBuilding::RenderGUI(void)
{

}

HRESULT CBuilding::AddComponent()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = Clone_Prototype(L"Proto_Transform");//CTransform::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	wstring key = L"transform_" + to_wstring(ID);
	m_mapComponent[ID_STATIC].insert({ key.c_str(), pComponent });

	pComponent = m_pMesh = CVoxel::Create(m_pGraphicDev, L"Building_01_object");
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Building_object", pComponent });

	return S_OK;
}
