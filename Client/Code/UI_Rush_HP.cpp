#include "stdafx.h"
#include "..\Header\UI_Rush_HP.h"
#include "Stage.h"
#include "Tank_01.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"

CUI_Rush_HP::CUI_Rush_HP(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Rush_HP::CUI_Rush_HP(const CUI_Rush_HP& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Rush_HP::~CUI_Rush_HP()
{
	//Free();
}

HRESULT CUI_Rush_HP::Ready_Object(void)
{
	std::fill(&m_pTexture[0], &m_pTexture[9], nullptr);
	std::fill(&m_pTransform[0], &m_pTransform[9], nullptr);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 15.f;
	m_fScaleY = 15.f;
	m_fScaleZ = 1.f;


	m_fPosX[0] = 15.f;
	m_fPosY[0] = 15.f;
	m_fPosZ = 0.1f;


	for (_int i = 0; i < 5; ++i)
	{
		m_fPosX[i] = 15.f + (30.f * i);
		m_fPosY[i] = 15.f;

		m_pTransform[i]->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform[i]->Set_Pos(m_fPosX[i] - WINCX_HALF, WINCY_HALF - m_fPosY[i], m_fPosZ);
	}
	for (_int i = 5; i < 10; ++i)
	{
		m_fPosX[i] =(30.f * i) - 135.f;
		m_fPosY[i] = 45.f;

		m_pTransform[i]->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform[i]->Set_Pos(m_fPosX[i] - WINCX_HALF, WINCY_HALF - m_fPosY[i], m_fPosZ);
	}

	m_LifeCount = CUI_FontMgr::GetInstance()->Get_Rush_LifeCount();
	
	for (_int i = 0; i < 10; ++i)
	{
		m_bShow[i] = true;
		if (i >= m_LifeCount)
		{
			m_bShow[i] = false;
		}
	}

	return S_OK;
}


_int CUI_Rush_HP::Update_Object(const _float & fTimeDelta)
{
	//m_LifeCount = CUI_FontMgr::GetInstance()->Get_Rush_LifeCount();

	//Key_Input(fTimeDelta);

	//Update_Life_Count();

	//__super::Update_Object(fTimeDelta);

	return OBJ_NOEVENT;
}

void CUI_Rush_HP::LateUpdate_Object(void)
{
	//__super::LateUpdate_Object();
	//
	//Add_RenderGroup(RENDER_UI, this);
	
}

void CUI_Rush_HP::Render_Object(void)
{
	/*	_matrix OldViewMatrix, OldProjMatrix;


	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);*/


	_matrix Origin_Proj, Origin_View , viewMatrix;
	D3DXMatrixIdentity(&viewMatrix);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &Origin_View);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &Origin_Proj);

	for (_int i = 0; i < 10; ++i)
	{
		if (m_bShow[i])
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform[i]->Get_WorldMatrix());
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &viewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
			m_pTexture[i]->Set_Texture(0);
			m_pRcTex->Render_Buffer();
		}
	}


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &Origin_View);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &Origin_Proj);
		
}

CUI_Rush_HP * CUI_Rush_HP::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Rush_HP*		pInstance = new CUI_Rush_HP(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Rush_HP Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Rush_HP::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });


	// Texture
#pragma region

	pComponent = m_pTexture[0] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[0], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex0", pComponent });

	pComponent = m_pTexture[1] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[1], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex1", pComponent });

	pComponent = m_pTexture[2] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[2], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex2", pComponent });

	pComponent = m_pTexture[3] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[3], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex3", pComponent });

	pComponent = m_pTexture[4] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[4], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex4", pComponent });

	pComponent = m_pTexture[5] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[5], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex5", pComponent });

	pComponent = m_pTexture[6] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[6], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex6", pComponent });

	pComponent = m_pTexture[7] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[7], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex7", pComponent });

	pComponent = m_pTexture[8] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[8], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex8", pComponent });

	pComponent = m_pTexture[9] = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Rush_HP_Tex"));
	NULL_CHECK_RETURN(m_pTexture[9], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Rush_HP_Tex9", pComponent });

#pragma endregion


	//Transform
#pragma region

	pComponent = m_pTransform[0] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[0], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP0", pComponent });

	pComponent = m_pTransform[1] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[1], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP1", pComponent });

	pComponent = m_pTransform[2] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[2], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP2", pComponent });

	pComponent = m_pTransform[3] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[3], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP3", pComponent });

	pComponent = m_pTransform[4] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[4], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP4", pComponent });

	pComponent = m_pTransform[5] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[5], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP5", pComponent });

	pComponent = m_pTransform[6] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[6], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP6", pComponent });

	pComponent = m_pTransform[7] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[7], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP7", pComponent });

	pComponent = m_pTransform[8] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[8], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP8", pComponent });

	pComponent = m_pTransform[9] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform[9], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rush_HP9", pComponent });


#pragma endregion

	return S_OK;
}

void CUI_Rush_HP::Free(void)
{
	for (_int i = 0; i < 10; ++i)
	{
		Safe_Release(m_pTexture[i]);
		Safe_Release(m_pTransform[i]);
	}


	CGameObject::Free();
}

void CUI_Rush_HP::Key_Input(const _float & fTimeDelta)
{
	
	if (Get_DIKeyState_Custom(DIK_NUMPADMINUS) == KEY_STATE::TAP)
	{
		CUI_FontMgr::GetInstance()->Rush_Minus_Life();
	}
	if (Get_DIKeyState_Custom(DIK_NUMPADPLUS) == KEY_STATE::TAP)
	{
		CUI_FontMgr::GetInstance()->Rush_Plus_Life();
	}
}

void CUI_Rush_HP::Update_Life_Count(void)
{
	for (_int i = 0; i < 10; ++i)
	{
		if (i < m_LifeCount)
		{
			m_bShow[i] = true;
		}
		else if (i >= m_LifeCount)
		{
			m_bShow[i] = false;
		}
	}
}
