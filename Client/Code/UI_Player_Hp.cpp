#include "stdafx.h"
#include "..\Header\UI_Player_Hp.h"
#include "Export_Function.h"

#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"


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
}

HRESULT CUI_Player_Hp::Ready_Object(_float fOrgin_HP)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj1, WINCX, WINCY, 0.f, 1.f);
	D3DXMatrixOrthoLH(&m_matProj2, WINCX, WINCY, 0.f, 1.f);

	m_fOrgin_HP = m_fHP = fOrgin_HP;


	// Front
	m_fOrgin_ScaleX = m_fScaleX = 70.f;
	m_fScaleY = 9.f;
	m_fScaleZ = 0.1f;

	m_fOrgin_PosX = m_fPosX = 150.f;
	m_fPosY = 468.f;
	m_fPosZ = 0.02f;


	// Back
	m_fBScaleX = 120.f;
	m_fBScaleY  = 40.f;
	m_fBScaleZ = 1.f;

	m_fBPosX  = 120.f;
	m_fBPosY  = 475.f;
	m_fBPosZ  = 0.03f;
	

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);


	m_pBTransform->Set_Scale(m_fBScaleX, m_fBScaleY, m_fBScaleZ);
	m_pBTransform->Set_Pos(m_fBPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fBPosY, m_fBPosZ);

	return S_OK;
}

_int CUI_Player_Hp::Update_Object(const _float & fTimeDelta)
{
	Update_HP();

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


		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
		_matrix	ViewMatrix1 , ViewMatrix2;
		D3DXMatrixIdentity(&ViewMatrix1);
		
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix1);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj1);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		D3DXMatrixIdentity(&ViewMatrix2);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBTransform->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix2);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj2);

		m_pBTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();

	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{
		m_pTransform->Set_Scale(1.f, 1.f, 1.f);
		m_pTransform->Set_Pos(1.f, 1.f, 1.f);
	}
}

CUI_Player_Hp * CUI_Player_Hp::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fOrgin_HP)
{
	CUI_Player_Hp*		pInstance = new CUI_Player_Hp(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(fOrgin_HP)))
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

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Player_Hp_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Hp_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_HP2", pComponent });



	pComponent = m_pBTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Player_Hp_Back_Tex"));
	NULL_CHECK_RETURN(m_pBTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Hp_Back_Tex", pComponent });

	pComponent = m_pBTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_HP1", pComponent });


	return S_OK;
}

void CUI_Player_Hp::Free(void)
{
	CGameObject::Free();
}

void CUI_Player_Hp::Update_HP(void)
{
	if (m_fHP > m_fOrgin_HP) { m_fHP = m_fOrgin_HP; }
	if (m_fHP < 0.f) { m_fHP = 0.f; }
	HP_Percent = (m_fHP / m_fOrgin_HP);
	if (HP_Percent > 1.f) { HP_Percent = 1.f; }
	if (HP_Percent < 0.f) { HP_Percent = 0.f; }
	m_fScaleX = (m_fOrgin_ScaleX * HP_Percent);

	_float fX = m_fOrgin_ScaleX - m_fScaleX;
	//	m_fPosX = (m_fOrgin_PosX *HP_Percent);
	m_fPosX = (m_fOrgin_PosX - fX);

}
