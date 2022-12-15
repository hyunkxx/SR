#include "stdafx.h"
#include "..\Header\UI_MiniMap.h"
#include "Stage.h"
#include "Tank_01.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "TankManager.h"
#include "UI_FontMgr.h"
#include "Humvee.h"
#include "SmallTank.h"
#include "MiddleTank.h"
#include "BigTank.h"
#include "LongTank.h"
CUI_MiniMap::CUI_MiniMap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_MiniMap::CUI_MiniMap(const CUI_MiniMap& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_MiniMap::~CUI_MiniMap()
{
}

HRESULT CUI_MiniMap::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 80.f;
	m_fScaleY = 80.f;
	m_fScaleZ = 1.f;

	m_fPosX = 715.f;
	m_fPosY = 515.f;
	m_fPosZ = 0.03f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
/*
	m_szTankType = CUI_FontMgr::GetInstance()->Get_Tank_Name();

	if (m_szTankType == L"Humvee")
	{
		m_iTankType = 0;
	}
	else if (m_szTankType == L"CV90 경전차")
	{
		m_iTankType = 1;
	}
	else if (m_szTankType == L"T62 중형전차")
	{
		m_iTankType = 2;
	}
	else if (m_szTankType == L"Tiger 중전차")
	{
		m_iTankType = 3;
	}
	else if (m_szTankType == L"K-9 자주곡사포")
	{
		m_iTankType = 4;
	}*/
	return S_OK;
}


_int CUI_MiniMap::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_MiniMap::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	
	Add_RenderGroup(RENDER_UI, this);
	
}

void CUI_MiniMap::Render_Object(void)
{


	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();

	}

}

CUI_MiniMap * CUI_MiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_MiniMap*		pInstance = new CUI_MiniMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Minimap Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_MiniMap::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_MiniMap_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_MiniMap_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_MiniMap", pComponent });


	return S_OK;
}

void CUI_MiniMap::Free(void)
{
	CGameObject::Free();
}
