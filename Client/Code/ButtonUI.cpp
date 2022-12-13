#include "stdafx.h"
#include "..\Header\ButtonUI.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"

CButtonUI::CButtonUI(LPDIRECT3DDEVICE9 pGraphicDev, VEHICLE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bUpdateTransform(true)
	, m_eType(eType)
{
}

CButtonUI::CButtonUI(const CButtonUI& rhs)
	: Engine::CGameObject(rhs)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bUpdateTransform(true)
	, m_eType(rhs.m_eType)
{
}


CButtonUI::~CButtonUI()
{
}

CButtonUI* CButtonUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, VEHICLE eType)
{
	CButtonUI*	pInstance = new CButtonUI(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CButtonUI Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CButtonUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 100.f;
	m_fScaleY = 80.f;
	m_fScaleZ = 1.f;

	m_fPosX = WINCX - m_fScaleX;
	m_fPosY = 200.f;
	m_fPosZ = 0.1f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CButtonUI::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	KeyInput();

	OnMouseClick();
	UpdateTransform();

	return 0;
}

void CButtonUI::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CButtonUI::Render_Object(void)
{
	if (!m_bShow)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix, OldProjection;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		
	m_bClicked ? m_pClickedTexture->Set_Texture(0) : m_pNonClickTexture->Set_Texture(0);

	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}

void CButtonUI::Free(void)
{
	__super::Free();
}

HRESULT CButtonUI::Add_Component(void)
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
	case Engine::VEHICLE::HUMVEE:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"humvee_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"humvee_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case Engine::VEHICLE::SMALL_TANK:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"small_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"small_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case Engine::VEHICLE::MIDDLE_TANK:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"middle_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"middle_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case Engine::VEHICLE::BIG_TANK:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"big_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"big_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case Engine::VEHICLE::LONG_TANK:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"long_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"long_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	default:
		MSG_BOX("CButtonUI 타입범위 초과");
		break;
	}

	return S_OK;
}

void CButtonUI::KeyInput()
{
	if (Get_DIKeyState_Custom(DIK_O) == KEY_STATE::TAP)
	{
		m_bShow = !m_bShow;

		//커서보이게 하고 차량 움직임 정지 (포신의 회전각만 적용됨)
		ShowCursor(m_bShow);
		static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Set_Rock(m_bShow);
	}
}

bool CButtonUI::OnMouseClick()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rc; 
	rc.left = (LONG)(m_fPosX - m_fScaleX * 0.5f);
	rc.right = (LONG)(m_fPosX + m_fScaleX * 0.5f);
	rc.top = (LONG)(m_fPosY - m_fScaleY * 0.5f);
	rc.bottom = (LONG)(m_fPosY + m_fScaleY * 0.5f);

	if (PtInRect(&rc, ptMouse) && Get_DIMouseState(DIM_LB) & 0x80)
	{
		m_bClicked = true;
	}
	else
	{
		m_bClicked = false;
	}

	return m_bClicked;
}

void CButtonUI::UpdateTransform()
{
	if (m_bUpdateTransform)
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
		m_bUpdateTransform = false;
	}
}
