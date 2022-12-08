#include "stdafx.h"
#include "..\Header\Building.h"

#include "Utility.h"
#include "Export_Function.h"
#include "Component.h"
#include "Transform.h"
#include "Voxel.h"

USING(Engine)

_int CBuilding::ID = -1;

CBuilding* CBuilding::Create(LPDIRECT3DDEVICE9 pGramphicDev, wstring strMeshKey, const _vec3& vPos)
{
	ID++;
	CBuilding* pInstance = new CBuilding(pGramphicDev, strMeshKey, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Building::Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuilding::Free(void)
{
	__super::Free();
}

CBuilding::~CBuilding()
{
}

CBuilding::CBuilding(LPDIRECT3DDEVICE9 pGramphicDev, wstring strMeshKey, const _vec3& vPos)
	: CGameObject(pGramphicDev)
	, m_vPosition(vPos)
	, m_strMeshKey(strMeshKey)
{
}

CBuilding::CBuilding(const CBuilding& rhs)
	: CGameObject(rhs)
	, m_vPosition(rhs.m_vPosition)
	, m_strMeshKey(rhs.m_strMeshKey)
{
}

HRESULT CBuilding::Ready_Object(void)
{
	m_ID = L"building" + to_wstring(ID);
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	D3DXMatrixIdentity(&m_matWorld);
	_matrix matRot, matTrans;

	D3DXMatrixRotationY(&matRot, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matRot *  matTrans;

	return S_OK;
}

void CBuilding::SetRotation(const _vec3& vRotation)
{
	m_vRotation = vRotation;
	_matrix matRot[3], matTrans;
	D3DXMatrixRotationX(&matRot[0], m_vRotation.x);
	D3DXMatrixRotationY(&matRot[1], m_vRotation.y);
	D3DXMatrixRotationZ(&matRot[2], m_vRotation.z);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matRot[0] * matRot[1] * matRot[2] * matTrans;
}

void CBuilding::SetPosition(const _vec3& vPos)
{
	m_vPosition = vPos;
	_matrix matRot[3], matTrans;
	D3DXMatrixRotationX(&matRot[0], m_vRotation.x);
	D3DXMatrixRotationY(&matRot[1], m_vRotation.y);
	D3DXMatrixRotationZ(&matRot[2], m_vRotation.z);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matRot[0] * matRot[1] * matRot[2] * matTrans;
}

_int CBuilding::Update_Object(const _float & fTimeDelta)
{
	//if (Utility::Cuilling(m_pGraphicDev, m_vPosition))
	//	return 0;

	__super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CBuilding::LateUpdate_Object(void)
{
	//if (Utility::Cuilling(m_pGraphicDev, m_vPosition))
	//	return;

	__super::LateUpdate_Object();
}

void CBuilding::Render_Object(void)
{
	//if (Utility::Cuilling(m_pGraphicDev, m_vPosition))
	//	return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pMesh->Render(&m_matWorld);
}

void CBuilding::RenderGUI(void)
{

}

HRESULT CBuilding::AddComponent()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = Clone_Prototype(L"Proto_Transform");
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	wstring key = L"transform_" + to_wstring(ID);
	m_mapComponent[ID_STATIC].insert({ key.c_str(), pComponent });

	pComponent = m_pMesh = CVoxel::Create(m_pGraphicDev, m_strMeshKey.c_str());
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Building_object", pComponent });

	return S_OK;
}
