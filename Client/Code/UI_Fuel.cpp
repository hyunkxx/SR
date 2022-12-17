#include "stdafx.h"
#include "..\Header\UI_Fuel.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"

#include "Humvee.h"
#include "SmallTank.h"
#include "MiddleTank.h"
#include "BigTank.h"
#include "LongTank.h"
#include "GameMode.h"

CUI_Fuel::CUI_Fuel(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Fuel::CUI_Fuel(const CUI_Fuel& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Fuel::~CUI_Fuel()
{
}

HRESULT CUI_Fuel::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj1, WINCX, WINCY, 0.f, 1.f);
	D3DXMatrixOrthoLH(&m_matProj2, WINCX, WINCY, 0.f, 1.f);


	m_fScaleX = 25.f;
	m_fScaleY = 65.f;
	m_fScaleZ = 1.f;

	m_fPosX = 225.f;
	m_fPosY = 545.f;
	m_fPosZ = 0.02f;


	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_fFScaleX = 25.f;
	m_fFOrigin_ScaleY = m_fFScaleY = 65.f;
	m_fFScaleZ = 1.f;

	m_fFPosX = 225.f;
	m_fFOrigin_PosY = m_fFPosY = 545.f;
	m_fFPosZ = 0.02f;

	m_pFTransform->Set_Scale(m_fFScaleX, m_fFScaleY, m_fFScaleZ);
	m_pFTransform->Set_Pos(m_fFPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fFPosY, m_fFPosZ);

	m_szTankType = CUI_FontMgr::GetInstance()->Get_Tank_Name();

	if (m_szTankType == L"Humvee")
	{
		m_fOrigin_fuel = m_fFuel = 150.f;
	}
	else if (m_szTankType == L"CV90 경전차")
	{
		m_fOrigin_fuel = m_fFuel = 200.f;
	}
	else if (m_szTankType == L"T62 중형전차")
	{
		m_fOrigin_fuel = m_fFuel = 250.f;
	}
	else if (m_szTankType == L"Tiger 중전차")
	{
		m_fOrigin_fuel = m_fFuel = 300.f;
	}
	else if (m_szTankType == L"K-9 자주곡사포")
	{
		m_fOrigin_fuel = m_fFuel = 350.f;
	}


	return S_OK;
}

_int CUI_Fuel::Update_Object(const _float & fTimeDelta)
{
	Update_Fuel(fTimeDelta);

	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Fuel::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CUI_Fuel::Render_Object(void)
{
	if (CGameMode::GetInstance()->m_bGameEnd)
		return;

	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		m_pFTransform->Set_Scale(m_fFScaleX, m_fFScaleY, m_fFScaleZ);
		m_pFTransform->Set_Pos(m_fFPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fFPosY, m_fFPosZ);

		//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
		_matrix	ViewMatrix1, ViewMatrix2;
		D3DXMatrixIdentity(&ViewMatrix1);


		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix1);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj1);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pFTransform->Get_WorldMatrix());

		D3DXMatrixIdentity(&ViewMatrix2);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix2);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj2);

		m_pFTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();


		//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{

	}
}

CUI_Fuel * CUI_Fuel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Fuel*		pInstance = new CUI_Fuel(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Fuel Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Fuel::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Fuel_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Fuel_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Fuel", pComponent });

	pComponent = m_pFTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Fuel_Bar_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Fuel_Bar_Tex", pComponent });

	pComponent = m_pFTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_FuelBar", pComponent });
	/*
	Proto_Stat_Back_RcTex
	Proto_Player_Hp_Back_RcTex
	Proto_BlueTeam_Kill_Back_RcTex
	Proto_BlueTeam_Kill_RcTex
	Proto_RedTeam_Kill_Back_RcTex
	Proto_RedTeam_Kill_RcTex
	Proto_Player_Hp_RcTex
	Proto_Log_Back_RcTex


	Proto_Player_Hp_Back_Tex
	Proto_BlueTeam_Kill_Back_Tex
	Proto_BlueTeam_Kill_Tex
	Proto_RedTeam_Kill_Back_Tex
	Proto_RedTeam_Kill_Tex
	Proto_Player_Hp_Tex
	Proto_Log_Back_Tex
	*/

	return S_OK;
}

void CUI_Fuel::Free(void)
{
	CGameObject::Free();
}

void CUI_Fuel::Update_Fuel(const _float& fTimeDelta)
{
	CGameObject* pTank = Engine::Get_Object(L"GameLogic", L"PlayerVehicle");

	if (m_szTankType == L"Humvee")
	{
		TankInfo = static_cast<CHumvee*>(pTank)->Get_TankInfo();
	}
	else if (m_szTankType == L"CV90 경전차")
	{
		TankInfo = static_cast<CSmallTank*>(pTank)->Get_TankInfo();
	}
	else if (m_szTankType == L"T62 중형전차")
	{
		TankInfo = static_cast<CMiddleTank*>(pTank)->Get_TankInfo();
	}
	else if (m_szTankType == L"Tiger 중전차")
	{
		TankInfo = static_cast<CBigTank*>(pTank)->Get_TankInfo();
	}
	else if (m_szTankType == L"K-9 자주곡사포")
	{
		TankInfo = static_cast<CLongTank*>(pTank)->Get_TankInfo();
	}

	//현재 연료  = 지난 연료 - (이동 거리 * 0.001( 너무 빨리 닳아서))
	m_fFuel = m_fFuel - (TankInfo.fSpeed * (fTimeDelta * 0.1f));

	// 연료의 백분율 = 현재 연료 / 최대 연료
	_float Fuel_Percent = m_fFuel / m_fOrigin_fuel;

	// 만약 연료의 백분율이 바닥이면 바닥으로 고정해라.
	if (Fuel_Percent <= 0.f)
	{
		Fuel_Percent = 0.f;
	}

	//현재 연료 바 = 원래 연료 바 백분율 적용
	m_fFScaleY = m_fFOrigin_ScaleY * Fuel_Percent;

	//  Y값 저장 = (연료가 줄어든 크기) 의 절반
	_float fY = (m_fFOrigin_ScaleY - m_fFScaleY) / 2.f;

	if (Fuel_Percent <= 0.9f)
	{
		fY = 1.2f * fY;
	}
	else if (Fuel_Percent <= 0.8f)
	{
		fY = 1.3f * fY;
	}
	else if (Fuel_Percent <= 0.7f)
	{
		fY = 1.4f * fY;
	}
	else if (Fuel_Percent <= 0.6f)
	{
		fY = 1.5f * fY;
	}
	else if (Fuel_Percent <= 0.5f)
	{
		fY = 1.6f * fY;
	}
	else if (Fuel_Percent <= 0.4f)
	{
		fY = 1.7f * fY;
	}
	else if (Fuel_Percent <= 0.3f)
	{
		fY = 1.8f * fY;
	}
	else if (Fuel_Percent <= 0.2f)
	{
		fY = 1.9f * fY;
	}
	else if (Fuel_Percent <= 0.1f)
	{
		fY = 2.f * fY;
	}
	// 연료 막대 Y 위치를 Y만큼 더해서 고정한 것 처럼.
	m_fFPosY = (m_fFOrigin_PosY + fY);

}
