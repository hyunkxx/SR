#include "stdafx.h"
#include "..\Header\UI_Player_Hp_Back.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"

CUI_Player_Hp_Back::CUI_Player_Hp_Back(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Player_Hp_Back::CUI_Player_Hp_Back(const CUI_Player_Hp_Back& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Player_Hp_Back::~CUI_Player_Hp_Back()
{
}

HRESULT CUI_Player_Hp_Back::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	//m_fScaleX = 100.f;
	//m_fScaleY = 60.f;
	//m_fScaleZ = 1.f;

	//m_fPosX = 80.f;
	//m_fPosY = 30.f;
	//m_fPosZ = 0.01f;

	m_fScaleX = 100.f;
	m_fScaleY = 9.5f;
	m_fScaleZ = 1.f;

	m_fPosX = 100.f;
	m_fPosY = 20.f;
	m_fPosZ = 0.03f;


	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CUI_Player_Hp_Back::Update_Object(const _float & fTimeDelta)
{
	_vec3 vTemp;
	m_pTransform->Get_Info(INFO_POS, &vTemp);
	__super::Compute_ViewZ(&vTemp);
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Player_Hp_Back::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CUI_Player_Hp_Back::Render_Object(void)
{
	CGameObject* pTankView = Engine::Get_Object(L"Environment", L"TankCamera");
	CGameObject* pStaticView = Engine::Get_Object(L"Environment", L"StaticCamera");
	CGameObject* pAimView = Engine::Get_Object(L"Environment", L"AimCamera");

	if ((static_cast<CTankCamera*>(pTankView)->Get_CameraOn()) || static_cast<CStaticCamera*>(pStaticView)->Get_CameraOn())
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

		//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();
		//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	else if (static_cast<CAimCamera*>(pAimView)->Get_CameraOn())
	{
		m_pTransform->Set_Scale(1.f, 1.f, 1.f);
		m_pTransform->Set_Pos(1.f, 1.f, 1.f);
	}
}

CUI_Player_Hp_Back * CUI_Player_Hp_Back::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Player_Hp_Back*		pInstance = new CUI_Player_Hp_Back(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Player_Status_Back Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Player_Hp_Back::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_Player_Hp_Back_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Hp_Back_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Player_Hp_Back_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Hp_Back_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_HP1", pComponent });
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

void CUI_Player_Hp_Back::Free(void)
{
	CGameObject::Free();
}
