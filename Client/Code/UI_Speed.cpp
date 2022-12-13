#include "stdafx.h"
#include "..\Header\UI_Speed.h"
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

CUI_Speed::CUI_Speed(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Speed::CUI_Speed(const CUI_Speed& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Speed::~CUI_Speed()
{
}

HRESULT CUI_Speed::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj1, WINCX, WINCY, 0.f, 1.f);
	D3DXMatrixOrthoLH(&m_matProj2, WINCX, WINCY, 0.f, 1.f);


	m_fScaleX = 80.f;
	m_fScaleY = 25.f;
	m_fScaleZ = 1.f;

	m_fPosX = 715.f;
	m_fPosY = 460.f;
	m_fPosZ = 0.02f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	// 이걸 움직이면 된다.
	m_fFScaleX = 0.f;
	m_fFOrgin_ScaleX = 70.f;
	m_fFScaleY = 14.f;
	m_fFScaleZ = 1.f;

	m_fFOrgin_PosX = m_fFPosX = 705.f;
	m_fFPosY = 451.f;
	m_fFPosZ = 0.01f;

	m_pFTransform->Set_Scale(m_fFScaleX, m_fFScaleY, m_fFScaleZ);
	m_pFTransform->Set_Pos(m_fFPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fFPosY, m_fFPosZ);

	m_szTankType = CUI_FontMgr::GetInstance()->Get_Tank_Name();

	return S_OK;
}

_int CUI_Speed::Update_Object(const _float & fTimeDelta)
{
	Update_Speed();
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Speed::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();


	Add_RenderGroup(RENDER_UI, this);

}

void CUI_Speed::Render_Object(void)
{

	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		m_pFTransform->Set_Scale(m_fFScaleX, m_fFScaleY, m_fFScaleZ);
		m_pFTransform->Set_Pos(m_fFPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fFPosY, m_fFPosZ);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
		_matrix	ViewMatrix1;
		D3DXMatrixIdentity(&ViewMatrix1);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix1);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj1);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pFTransform->Get_WorldMatrix());
		_matrix	ViewMatrix2;
		D3DXMatrixIdentity(&ViewMatrix2);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix2);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj2);

		m_pFTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();
	}
	else if (Engine::Get_Camera_ID() ==CAMERA_ID::AIM_CAMERA)
	{
	
	}
}

CUI_Speed * CUI_Speed::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Speed*		pInstance = new CUI_Speed(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Speed Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Speed::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Speed_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Speed_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Speed", pComponent });


	pComponent = m_pFTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_SpeedBar_Tex"));
	NULL_CHECK_RETURN(m_pFTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_SpeedBar_Tex", pComponent });

	pComponent = m_pFTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pFTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_SpeedBar", pComponent });


	return S_OK;
}

void CUI_Speed::Free(void)
{
	CGameObject::Free();
}

void CUI_Speed::Update_Speed(void)
{
	CGameObject* pTank = Engine::Get_Object(L"GameLogic", L"PlayerVehicle");

	if (m_szTankType == L"Humvee")
	{
		TankInfo = static_cast<CHumvee*>(pTank)->Get_TankInfo();

		//	fSpeed = static_cast<CHumvee*>(pTank)->Get_Speed();
	}
	else if (m_szTankType == L"CV90 경전차")
	{
		TankInfo = static_cast<CSmallTank*>(pTank)->Get_TankInfo();
		//fSpeed = static_cast<CSmallTank*>(pTank)->Get_Speed();
	}
	else if (m_szTankType == L"T62 중형전차")
	{
		TankInfo = static_cast<CMiddleTank*>(pTank)->Get_TankInfo();
		//fSpeed = static_cast<CMiddleTank*>(pTank)->Get_Speed();
	}
	else if (m_szTankType == L"Tiger 중전차")
	{
		TankInfo = static_cast<CBigTank*>(pTank)->Get_TankInfo();
		//fSpeed = static_cast<CBigTank*>(pTank)->Get_Speed();
	}
	else if (m_szTankType == L"K-9 자주곡사포")
	{
		TankInfo = static_cast<CLongTank*>(pTank)->Get_TankInfo();
		//fSpeed = static_cast<CLongTank*>(pTank)->Get_Speed();
	}

	

	_float fSpeed_Percent = fabsf(TankInfo.fSpeed / TankInfo.fAccel_top_speed);
	
	if (fSpeed_Percent >= 1.f)
	{
		fSpeed_Percent = 1.f;
	}

	m_fFScaleX = m_fFOrgin_ScaleX * fSpeed_Percent;

	_float fX = (m_fFOrgin_ScaleX - m_fFScaleX) / 2.f;

	m_fFPosX = (m_fFOrgin_PosX + fX);


}
