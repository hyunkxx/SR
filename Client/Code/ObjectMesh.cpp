#include "stdafx.h"
#include "..\Header\ObjectMesh.h"

#include "Export_Function.h"

#include "Utility.h"
#include "Voxel.h"

USING(Engine)

CObjectMesh::CObjectMesh(LPDIRECT3DDEVICE9 pGraphic, wstring strMeshName)
	: CGameObject(pGraphic)
	, m_bSeleted(false)
	, m_fAngle(230.f)
	, m_strMeshName(strMeshName)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(&m_vPosition, sizeof(_vec3));
}

CObjectMesh::~CObjectMesh()
{
}

void CObjectMesh::Free()
{
	__super::Free();
}

_int CObjectMesh::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	KeyInput();

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	_vec3 camPos = { matView._41, matView._42, matView._43 };

	if (m_bSeleted)
	{
		m_vPosition.x = Utility::Lerp(m_vPosition.x, m_vViewPosition.x, 5.f * fTimeDelta);
		m_vPosition.y = Utility::Lerp(m_vPosition.y, m_vViewPosition.y, 5.f * fTimeDelta);
		m_vPosition.z = Utility::Lerp(m_vPosition.z, m_vViewPosition.z, fTimeDelta);

	}
	else
	{
		m_vPosition.x = Utility::Lerp(m_vPosition.x, m_vOriginPosition.x, 5.f * fTimeDelta);
		m_vPosition.y = Utility::Lerp(m_vPosition.y, m_vOriginPosition.y, 5.f * fTimeDelta);
		m_vPosition.z = Utility::Lerp(m_vPosition.z, m_vOriginPosition.z, fTimeDelta);
	}

	if (m_bSeleted)
		Rotation(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	return _int();
}

void CObjectMesh::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CObjectMesh::Render_Object(void)
{
	_matrix matScale, matRot, matTrans;
	const _vec3 up = { 0.f , 1.f, 0.f };

	if (m_bSeleted)
	{
		D3DXMatrixScaling(&matScale, 1.3f, 1.3f, 1.3f);
		D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	}
	else
	{
		D3DXMatrixScaling(&matScale, .3f, .3f, .3f);
		D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	}

	D3DXMatrixRotationAxis(&matRot, &up, D3DXToRadian(m_fAngle));

	m_matWorld = matScale * matRot * matTrans;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pBodyMesh->Render(&m_matWorld);
	m_pHeadMesh->Render(&m_matWorld);
	m_PosinMesh->Render(&m_matWorld);
}

void CObjectMesh::RenderGUI(void)
{
}

HRESULT CObjectMesh::Add_Component(void)
{
	wstring bodyKey = m_strMeshName + L"_ally_body";
	m_pBodyMesh = CVoxel::Create(m_pGraphicDev, bodyKey.c_str());
	m_mapComponent[ID_DYNAMIC].insert({ bodyKey.c_str(), m_pBodyMesh });

	wstring headKey = m_strMeshName + L"_ally_head";
	m_pHeadMesh = CVoxel::Create(m_pGraphicDev, headKey.c_str());
	m_mapComponent[ID_DYNAMIC].insert({ headKey.c_str(), m_pHeadMesh });

	wstring posinKey = m_strMeshName + L"_ally_posin";
	m_PosinMesh = CVoxel::Create(m_pGraphicDev, posinKey.c_str());
	m_mapComponent[ID_DYNAMIC].insert({ posinKey.c_str(), m_PosinMesh });

	return S_OK;
}

HRESULT CObjectMesh::Ready_Object(void)
{
	Add_Component();

	ZeroMemory(&m_vViewPosition, sizeof(_vec3));
	m_vOriginPosition = { m_vPosition.x , m_vPosition.y, m_vPosition.z };
	m_vViewPosition = { m_vPosition.x - 4.f , m_vPosition.y + 8.f , m_vPosition.z };
	return S_OK;
}

void CObjectMesh::KeyInput()
{
}

void CObjectMesh::Rotation(const float& fDeltaTime)
{
	m_fAngle += 90 * fDeltaTime;
}

void CObjectMesh::Selected()
{
	m_bSeleted = true;
}

void CObjectMesh::UnSelect()
{
	m_bSeleted = false;
	m_fAngle = 230.f;
}

const _vec3 CObjectMesh::Get_Info(void)
{
	return m_vOriginPosition;
}

void CObjectMesh::Move_Info(_vec3 _Info)
{
}

CObjectMesh* CObjectMesh::Create(LPDIRECT3DDEVICE9 pGraphic, wstring strMeshName)
{
	CObjectMesh* pInstance = new CObjectMesh(pGraphic, strMeshName);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Failed CObjectMesh::Create()");
		Safe_Release(pInstance);
	}

	return pInstance;
}