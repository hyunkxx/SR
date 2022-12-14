#include "stdafx.h"
#include "..\Header\UI_Reload.h"
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

CUI_Reload::CUI_Reload(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Reload::CUI_Reload(const CUI_Reload& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Reload::~CUI_Reload()
{
}

HRESULT CUI_Reload::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj1, WINCX, WINCY, 0.f, 1.f);
	D3DXMatrixOrthoLH(&m_matProj2, WINCX, WINCY, 0.f, 1.f);


	m_fScaleX = 100.f;
	m_fScaleY = 35.f;
	m_fScaleZ = 1.f;

	m_fPosX = 720.f;
	m_fPosY = 400.f;
	m_fPosZ = 0.02f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	// 이걸 움직이면 된다.
	m_fFScaleX = 0.f;
	m_fFOrgin_ScaleX = 70.f;
	m_fFScaleY = 14.f;
	m_fFScaleZ = 1.f;

	m_fFOrgin_PosX = m_fFPosX = 705.f;
	m_fFPosY = 430.f;
	m_fFPosZ = 0.01f;

	m_pFTransform->Set_Scale(m_fFScaleX, m_fFScaleY, m_fFScaleZ);
	m_pFTransform->Set_Pos(m_fFPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fFPosY, m_fFPosZ);

	m_szTankType = CUI_FontMgr::GetInstance()->Get_Tank_Name();

	return S_OK;
}

_int CUI_Reload::Update_Object(const _float & fTimeDelta)
{
	Update_Speed();
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Reload::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();


	Add_RenderGroup(RENDER_UI, this);

}

void CUI_Reload::Render_Object(void)
{

	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		m_fTime = m_fTime + 0.05f;
		wstring szReload{ L"ReLoading" };
		if (m_fTime >= 7.f)
		{
			szReload = L"ReLoading...";
		}
		else if (m_fTime >= 1.f)
		{
			m_fTime = 0.f;
			szReload = L"!!Ready!!";
		}
		if (Get_DIMouseState(DIM_LB))
		{
			szReload = L"발 사";
		}


		Render_Font(L"Font_Retro", (szReload).c_str(), &_vec2(550.f, 570.f), D3DXCOLOR(m_fTime, 1.f - m_fTime, 1.f - m_fTime, 1.f));


		m_pFTransform->Set_Scale(m_fFScaleX, m_fFScaleY, m_fFScaleZ);
		m_pFTransform->Set_Pos(m_fFPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fFPosY, m_fFPosZ);

		_matrix OldViewMatrix, OldProjMatrix, Minimap_ViewMatrix;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);


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

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);


	}
	else if (Engine::Get_Camera_ID() ==CAMERA_ID::AIM_CAMERA)
	{
	
	}
}

CUI_Reload * CUI_Reload::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Reload*		pInstance = new CUI_Reload(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Reload Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Reload::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Reload_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Reload_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Reload", pComponent });


	pComponent = m_pFTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_ReloadBar_Tex"));
	NULL_CHECK_RETURN(m_pFTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_ReloadBar_Tex", pComponent });

	pComponent = m_pFTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pFTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_ReloadBar", pComponent });


	return S_OK;
}

void CUI_Reload::Free(void)
{
	CGameObject::Free();
}

void CUI_Reload::Update_Speed(void)
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

	//_float fSpeed_Percent = fabsf(TankInfo.fSpeed / TankInfo.fAccel_top_speed);



	//if (fSpeed_Percent >= 1.f)
	//{
	//	fSpeed_Percent = 1.f;
	//}

	//m_fFScaleX = m_fFOrgin_ScaleX * fSpeed_Percent;

	//_float fX = (m_fFOrgin_ScaleX - m_fFScaleX) / 2.f;

	//m_fFPosX = (m_fFOrgin_PosX + fX);


}
