#include "stdafx.h"
#include "..\Header\Grass.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"

int CGrass::ID = 0;

CGrass::CGrass(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_bUpdateTransform(true)
	, m_eType(eType)
{
}

CGrass::CGrass(const CGrass& rhs)
	: Engine::CGameObject(rhs)
	, m_bUpdateTransform(true)
	, m_eType(rhs.m_eType)
{
}


CGrass::~CGrass()
{
}

CGrass* CGrass::Create(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType)
{
	ID++;
	CGrass*	pInstance = new CGrass(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGrass Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CGrass::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_ID = L"Grass" + to_wstring(ID);

	switch (m_eType)
	{
	case CGrass::TYPE::GRASS1:
		m_fScaleX = 5.f;
		m_fScaleY = 5.f;
		m_fScaleZ = 1.f;

		m_fPosX = 0.f;
		m_fPosY = 0.f;
		m_fPosZ = 0.f;
		break;
	case CGrass::TYPE::GRASS2:
		m_fScaleX = 5.f;
		m_fScaleY = 5.f;
		m_fScaleZ = 1.f;

		m_fPosX = 0.f;
		m_fPosY = 0.f;
		m_fPosZ = 0.f;
		break;
	default:
		break;
	}

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX, m_fPosY, m_fPosZ);

	return S_OK;
}

_int CGrass::Update_Object(const _float & fTimeDelta)
{
	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	if (Utility::Cuilling(m_pGraphicDev, vPos))
		return 0;

	__super::Update_Object(fTimeDelta);
	UpdateTransform();

	return 0;
}

void CGrass::LateUpdate_Object(void)
{
	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	if (Utility::Cuilling(m_pGraphicDev, vPos))
		return;

	__super::LateUpdate_Object();
	Add_RenderGroup(RENDER_ALPHA, this);
}

void CGrass::Render_Object(void)
{
	RenderImage();
}

void CGrass::Free(void)
{
	__super::Free();
}

HRESULT CGrass::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex1 = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex1, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"rect_textureOne", pComponent });

	pComponent = m_pRcTex2 = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex2, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"rect_textureTwo", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform", pComponent });

	pComponent = m_pTransformTwo = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformTwo, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transformTwo", pComponent });

	switch (m_eType)
	{
	case CGrass::TYPE::GRASS1:
		pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Grass"));
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });
		break;
	case CGrass::TYPE::GRASS2:
		pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Grass2"));
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });
		break;
	default:
		break;
	}


	return S_OK;
}

void CGrass::UpdateTransform()
{
	if (m_bUpdateTransform)
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX, m_fPosY, m_fPosZ);
		m_pTransform->Rotation(ROTATION::ROT_Y, D3DXToRadian(iRotaion));

		m_pTransformTwo->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransformTwo->Set_Pos(m_fPosX, m_fPosY, m_fPosZ);
		m_pTransformTwo->Rotation(ROTATION::ROT_Y, D3DXToRadian(iRotaion + 90.f));

		m_bUpdateTransform = false;
	}
}

void CGrass::RenderImage()
{
	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	if (Utility::Cuilling(m_pGraphicDev, vPos))
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
	m_pTexture->Set_Texture(0);
	m_pRcTex1->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformTwo->Get_WorldMatrix());
	m_pTexture->Set_Texture(0);
	m_pRcTex1->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}