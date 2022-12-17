#include "stdafx.h"
#include "..\Header\ModeButton.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"

CModeButton::CModeButton(LPDIRECT3DDEVICE9 pGraphicDev, GMODE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(eType)
{
}

CModeButton::CModeButton(const CModeButton& rhs)
	: Engine::CGameObject(rhs)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(rhs.m_eType)
{
}


CModeButton::~CModeButton()
{
}

CModeButton* CModeButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, GMODE eType)
{
	CModeButton*	pInstance = new CModeButton(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CModeButton Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CModeButton::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 130.f;
	m_fScaleY = 130.f;
	m_fScaleZ = 0.01f;

	m_fPosX = 0.f;
	m_fPosY = WINCY * 0.5f;
	m_fPosZ = 0.1f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CModeButton::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	KeyInput();

	if (OnMouseClick())
	{
		m_bClicked = false;
	}

	UpdateTransform();

	return 0;
}

void CModeButton::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CModeButton::Render_Object(void)
{
	RenderButton();
}

void CModeButton::Free(void)
{
	__super::Free();
}

HRESULT CModeButton::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"rect_texture", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform", pComponent });


	switch (m_eType)
	{
	case GMODE::WAR:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"btn_war"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"btn_war_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case GMODE::RUSH:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"btn_rush"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"btn_rush_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	default:
		MSG_BOX("CModeButton 타입범위 초과");
		break;
	}

	return S_OK;
}

void CModeButton::KeyInput()
{
}

bool CModeButton::OnMouseClick()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rc;
	rc.left = (LONG)(m_fPosX - m_fScaleX * 0.8f);
	rc.right = (LONG)(m_fPosX + m_fScaleX * 0.8f);
	rc.top = (LONG)(m_fPosY - m_fScaleY * 0.8f);
	rc.bottom = (LONG)(m_fPosY + m_fScaleY * 0.8f);

	if (m_eType == GMODE::WAR)
	{
		if (PtInRect(&rc, ptMouse) && ImGui::IsKeyReleased(ImGuiKey_MouseLeft))
		{
			if (CGameMode::GetInstance()->m_bRushPressed)
				CGameMode::GetInstance()->m_bRushPressed = false;

			CGameMode::GetInstance()->m_bWarPressed = !CGameMode::GetInstance()->m_bWarPressed;
		}
	}
	else if (m_eType == GMODE::RUSH)
	{
		if (PtInRect(&rc, ptMouse) && ImGui::IsKeyReleased(ImGuiKey_MouseLeft))
		{
			if (CGameMode::GetInstance()->m_bWarPressed)
				CGameMode::GetInstance()->m_bWarPressed = false;

			CGameMode::GetInstance()->m_bRushPressed = !CGameMode::GetInstance()->m_bRushPressed;
		}
	}


	if (PtInRect(&rc, ptMouse) && ImGui::IsKeyReleased(ImGuiKey_MouseLeft))
		m_bClicked = true;
	else
		m_bClicked = false;

	return m_bClicked;
}

void CModeButton::UpdateTransform()
{
	if (m_bUpdateTransform)
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
		m_bUpdateTransform = false;
	}
}

void CModeButton::RenderButton()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix, OldProjection;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	switch (m_eType)
	{
	case GMODE::WAR:
		CGameMode::GetInstance()->m_bWarPressed ?
			m_pClickedTexture->Set_Texture(0) : m_pNonClickTexture->Set_Texture(0);
		break;
	case GMODE::RUSH:
		CGameMode::GetInstance()->m_bRushPressed ?
			m_pClickedTexture->Set_Texture(0) : m_pNonClickTexture->Set_Texture(0);
		break;
	}


	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}