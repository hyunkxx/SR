#include "stdafx.h"
#include "..\Header\UI_Player_Hp.h"
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

CUI_Player_Hp::CUI_Player_Hp(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Player_Hp::CUI_Player_Hp(const CUI_Player_Hp& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Player_Hp::~CUI_Player_Hp()
{
	//Free();
}

HRESULT CUI_Player_Hp::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	// 채팅창 살릴려면 이거 복구
#pragma region
	//// Front
	//m_fOrgin_ScaleX = m_fScaleX = 70.f;
	//m_fScaleY = 9.f;
	//m_fScaleZ = 0.1f;

	//m_fOrgin_PosX = m_fPosX = 150.f;
	//m_fPosY = 468.f;
	//m_fPosZ = 0.02f;


	//// Back
	//m_fBScaleX = 120.f;
	//m_fBScaleY  = 40.f;
	//m_fBScaleZ = 1.f;

	//m_fBPosX  = 120.f;
	//m_fBPosY  = 475.f;
	//m_fBPosZ  = 0.03f;
	//
	//// Reload

	//m_fROrgin_ScaleX = m_fRScaleX = 50.f;
	//m_fRScaleY = 4.f;
	//m_fRScaleZ = 0.1f;

	//m_fROrgin_PosX = m_fRPosX = 135.f;
	//m_fRPosY = 485.f;
	//m_fRPosZ = 0.02f;


	//// Hp bar
	//m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	//m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	//// Hp Back
	//m_pBTransform->Set_Scale(m_fBScaleX, m_fBScaleY, m_fBScaleZ);
	//m_pBTransform->Set_Pos(m_fBPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fBPosY, m_fBPosZ);

	//// Reload
	//m_pRTransform->Set_Scale(m_fRScaleX, m_fRScaleY, m_fRScaleZ);
	//m_pRTransform->Set_Pos(m_fRPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fRPosY, m_fRPosZ);


	//// Player Tank Info Get
	//szTankName = CUI_FontMgr::GetInstance()->Get_Tank_Name();

#pragma endregion


	// 채팅창 없애고 위치 지정
#pragma region
	// Front
	m_fOrgin_ScaleX = m_fScaleX = 70.f;
	m_fScaleY = 9.f;
	m_fScaleZ = 0.1f;

	m_fOrgin_PosX = m_fPosX = 150.f;
	m_fPosY = 558.f;
	m_fPosZ = 0.02f;


	// Back
	m_fBScaleX = 120.f;
	m_fBScaleY = 40.f;
	m_fBScaleZ = 1.f;

	m_fBPosX = 120.f;
	m_fBPosY = 565.f;
	m_fBPosZ = 0.03f;

	// Reload

	m_fROrgin_ScaleX = m_fRScaleX = 50.f;
	m_fRScaleY = 4.f;
	m_fRScaleZ = 0.1f;

	m_fROrgin_PosX = m_fRPosX = 135.f;
	m_fRPosY = 575.f;
	m_fRPosZ = 0.02f;


	// Hp bar
	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	// Hp Back
	m_pBTransform->Set_Scale(m_fBScaleX, m_fBScaleY, m_fBScaleZ);
	m_pBTransform->Set_Pos(m_fBPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fBPosY, m_fBPosZ);

	// Reload
	m_pRTransform->Set_Scale(m_fRScaleX, m_fRScaleY, m_fRScaleZ);
	m_pRTransform->Set_Pos(m_fRPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fRPosY, m_fRPosZ);


	// Player Tank Info Get
	szTankName = CUI_FontMgr::GetInstance()->Get_Tank_Name();


#pragma endregion





	return S_OK;
}

_int CUI_Player_Hp::Update_Object(const _float & fTimeDelta)
{
	Update_Tank_Info();
	Update_HP();
	Update_Reload();
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Player_Hp::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CUI_Player_Hp::Render_Object(void)
{

	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA )
	{
		_int iPercent = _int(HP_Percent * 100.f);

		Render_Font(L"Font_Retro3", (to_wstring(iPercent)).c_str(), &_vec2(m_fOrgin_PosX - PERCENTX * 15.f, m_fPosY + PERCENTY * 0.5f), CUI_FontMgr::GetInstance()->Get_Hecks_B());

		//_matrix OldViewMatrix, OldProjMatrix;
		//m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		// Hp bar
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

		// Reload Bar
		m_pRTransform->Set_Scale(m_fRScaleX, m_fRScaleY, m_fRScaleZ);
		m_pRTransform->Set_Pos(m_fRPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fRPosY, m_fRPosZ);


		// HP _Bar
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();

		// Reload
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pRTransform->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		m_pRTexture->Set_Texture(Reload_Fire);
		m_pRcTex->Render_Buffer();

		// HP _Back
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBTransform->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		m_pBTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();
		
		//m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{
		m_pTransform->Set_Scale(1.f, 1.f, 1.f);
		m_pTransform->Set_Pos(1.f, 1.f, 1.f);
	}
}

CUI_Player_Hp * CUI_Player_Hp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Player_Hp*		pInstance = new CUI_Player_Hp(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Player_Status_Back Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Player_Hp::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//Hp Bar
	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Player_Hp_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Hp_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_HP2", pComponent });


	// Hp Back
	pComponent = m_pBTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Player_Hp_Back_Tex"));
	NULL_CHECK_RETURN(m_pBTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Hp_Back_Tex", pComponent });

	pComponent = m_pBTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_HP1", pComponent });


	// Reload
	pComponent = m_pRTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Reload_Tex"));
	NULL_CHECK_RETURN(m_pRTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Reload_Tex", pComponent });

	pComponent = m_pRTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pRTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Reload", pComponent });


	return S_OK;
}

void CUI_Player_Hp::Free(void)
{
	//Safe_Release(m_pRTexture);
	//Safe_Release(m_pRTransform);
	CGameObject::Free();
}

void CUI_Player_Hp::Update_Tank_Info(void)
{
	CGameObject* pTank = Engine::Get_Object(L"GameLogic", L"PlayerVehicle");

	if (szTankName == L"Humvee")
	{
		TankInfo = static_cast<CHumvee*>(pTank)->Get_TankInfo();
	}
	else if (szTankName == L"A - 44")
	{
		TankInfo = static_cast<CSmallTank*>(pTank)->Get_TankInfo();
	}
	else if (szTankName == L"TE6E4")
	{
		TankInfo = static_cast<CMiddleTank*>(pTank)->Get_TankInfo();
	}
	else if (szTankName == L"KING TIGER")
	{
		TankInfo = static_cast<CBigTank*>(pTank)->Get_TankInfo();
	}
	else if (szTankName == L"M55")
	{
		TankInfo = static_cast<CLongTank*>(pTank)->Get_TankInfo();
	}
}

void CUI_Player_Hp::Update_HP(void)
{
	HP_Percent = (TankInfo.fCurHP / TankInfo.fMaxHP);
	if (HP_Percent >= 1.f) { HP_Percent = 1.f; }
	if (HP_Percent <= 0.f) { HP_Percent = 0.f; }
	m_fScaleX = (m_fOrgin_ScaleX * HP_Percent);

	_float fX = m_fOrgin_ScaleX - m_fScaleX;
	//	m_fPosX = (m_fOrgin_PosX *HP_Percent);
	m_fPosX = (m_fOrgin_PosX - fX);

}

void CUI_Player_Hp::Update_Reload(void)
{

	Reload_Percent = (TankInfo.fReloadTime / TankInfo.fReload);

	if (Reload_Percent >= 1.f)
	{
		Reload_Percent = 1.f;
		Reload_Fire = true;
	}
	else if (Reload_Percent < 1.f)
	{
		Reload_Fire = false;
	}

	if (Reload_Percent <= 0.f)
	{
		Reload_Percent = 0.f;
	}

	m_fRScaleX = (m_fROrgin_ScaleX * Reload_Percent);

	_float fX = m_fROrgin_ScaleX - m_fRScaleX;

	m_fRPosX = m_fROrgin_PosX - fX;

}
