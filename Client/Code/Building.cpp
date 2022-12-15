#include "stdafx.h"
#include "..\Header\Building.h"

#include "Utility.h"
#include "Export_Function.h"
#include "Component.h"
#include "Transform.h"
#include "Voxel.h"

USING(Engine)

_int CBuilding::ID = -1;

CBuilding* CBuilding::Create(LPDIRECT3DDEVICE9 pGramphicDev, wstring strMeshKey, const _vec3& vPos, TYPE eType)
{
	ID++;
	CBuilding* pInstance = new CBuilding(pGramphicDev, strMeshKey, vPos, eType);

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

CBuilding::CBuilding(LPDIRECT3DDEVICE9 pGramphicDev, wstring strMeshKey, const _vec3& vPos, TYPE eType)
	: CGameObject(pGramphicDev)
	, m_vPosition(vPos)
	, m_strMeshKey(strMeshKey)
	, m_eType(eType)
{
}

CBuilding::CBuilding(const CBuilding& rhs)
	: CGameObject(rhs)
	, m_vPosition(rhs.m_vPosition)
	, m_strMeshKey(rhs.m_strMeshKey)
	, m_eType(rhs.m_eType)
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

	CollisionSetting();

	static_cast<CTransform*>(m_pTransform)->Set_Pos(m_vPosition.x, m_vPosition.y, m_vPosition.z);
	CGameObject::Ready_Object();
	Update_OBB();

	return S_OK;
}

void CBuilding::SetRotation(const _float& fRotation)
{
	static_cast<CTransform*>(m_pTransform)->Set_Pos(m_vPosition.x, m_vPosition.y, m_vPosition.z);
	static_cast<CTransform*>(m_pTransform)->Rotation(ROTATION::ROT_Y, m_fRotation);
}

void CBuilding::SetPosition(const _vec3& vPos)
{
	static_cast<CTransform*>(m_pTransform)->Set_Pos(m_vPosition.x, m_vPosition.y, m_vPosition.z);
	static_cast<CTransform*>(m_pTransform)->Rotation(ROTATION::ROT_Y, m_fRotation);
}

_int CBuilding::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	Update_OBB();

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CBuilding::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBuilding::Render_Object(void)
{
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
	m_mapComponent[ID_DYNAMIC].insert({ key.c_str(), pComponent });

	pComponent = m_pMesh = CVoxel::Create(m_pGraphicDev, m_strMeshKey.c_str());
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"mesh_component", pComponent });

	return S_OK;
}

void CBuilding::CollisionSetting()
{
	switch (m_eType)
	{
	case TYPE::BUILDING:
		m_stBody.fLen[x] = 15.f;
		m_stBody.fLen[y] = 14.f;
		m_stBody.fLen[z] = 10.5;
		break;
	case TYPE::ROCK:
		m_stBody.fLen[x] = 10.f;
		m_stBody.fLen[y] = 4.f;
		m_stBody.fLen[z] = 6.f;
		break;
	case TYPE::BASE:
		m_stBody.fLen[x] = 15.f;
		m_stBody.fLen[y] = 14.f;
		m_stBody.fLen[z] = 10.f;
		break;
	case TYPE::OASIS:
		m_stBody.fLen[x] = 2.f;
		m_stBody.fLen[y] = 3.f;
		m_stBody.fLen[z] = 2.f;
		break;
	default:
		m_stBody.fLen[x] = 2.f;
		m_stBody.fLen[y] = 3.f;
		m_stBody.fLen[z] = 2.f;
		break;
	}
}

/* ICollisionable */
const   _vec3 CBuilding::Get_Info(void)
{
	return m_vPosition;
}

void  CBuilding::Move_Info(_vec3 _Info)
{

}

void  CBuilding::OBB_Collision_EX(void)
{
}

void  CBuilding::Update_OBB(void)
{
	_vec3 Pos, Right, Up, Look;

	static_cast<CTransform*>(m_pTransform)->Get_Info(INFO_POS, &m_stBody.vPos);
	static_cast<CTransform*>(m_pTransform)->Get_Info(INFO_RIGHT, &m_stBody.vDir[x]);
	static_cast<CTransform*>(m_pTransform)->Get_Info(INFO_UP, &m_stBody.vDir[y]);
	static_cast<CTransform*>(m_pTransform)->Get_Info(INFO_LOOK, &m_stBody.vDir[z]);

	for (_int i = 0; i < 3; i++)
		D3DXVec3Normalize(&m_stBody.vDir[i], &m_stBody.vDir[i]);
}

OBB* CBuilding::Get_OBB()
{
	return &m_stBody;
}
