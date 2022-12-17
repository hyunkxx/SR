#include "stdafx.h"
#include "..\Header\GameButton.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"

CGameButton::CGameButton(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(eType)
{
}

CGameButton::CGameButton(const CGameButton& rhs)
	: Engine::CGameObject(rhs)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(rhs.m_eType)
{
}


CGameButton::~CGameButton()
{
}

CGameButton* CGameButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType)
{
	CGameButton*	pInstance = new CGameButton(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGameButton Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CGameButton::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 100.f;
	m_fScaleY = 30.f;
	m_fScaleZ = 0.01f;

	m_fPosX = 0.f;
	m_fPosY = WINCY * 0.5f;
	m_fPosZ = 0.1f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CGameButton::Update_Object(const _float & fTimeDelta)
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

void CGameButton::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CGameButton::Render_Object(void)
{
	RenderButton();
}

void CGameButton::Free(void)
{
	__super::Free();
}

HRESULT CGameButton::Add_Component(void)
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
	case TYPE::START:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"btn_start"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });
		break;
	case TYPE::EXIT:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"btn_exit"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });
		break;
	default:
		MSG_BOX("CGameButton 타입범위 초과");
		break;
	}

	return S_OK;
}

void CGameButton::KeyInput()
{
}

bool CGameButton::OnMouseClick()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rc;
	rc.left = (LONG)(m_fPosX - m_fScaleX * 0.8f);
	rc.right = (LONG)(m_fPosX + m_fScaleX * 0.8f);
	rc.top = (LONG)(m_fPosY - m_fScaleY * 0.8f);
	rc.bottom = (LONG)(m_fPosY + m_fScaleY * 0.8f);

	if (PtInRect(&rc, ptMouse) && Get_DIMouseState(DIM_LB) & 0x80)
	{
		m_bPressed = true;
	}
	else
	{
		m_bPressed = false;
	}

	if (PtInRect(&rc, ptMouse) && ImGui::IsKeyReleased(ImGuiKey_MouseLeft))
		m_bClicked = true;
	else
		m_bClicked = false;

	switch (m_eType)
	{
	case CGameButton::TYPE::START:
		CGameMode::GetInstance()->m_bGameStart = m_bClicked;
		break;
	case CGameButton::TYPE::EXIT:
		CGameMode::GetInstance()->m_bGameExit = m_bClicked;
		break;
	default:
		break;
	}

	return m_bClicked;
}

void CGameButton::UpdateTransform()
{
	if (m_bUpdateTransform)
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
		m_bUpdateTransform = false;
	}
}

void CGameButton::RenderButton()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix, OldProjection;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pNonClickTexture->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}