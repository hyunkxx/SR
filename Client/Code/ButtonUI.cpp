#include "stdafx.h"
#include "..\Header\ButtonUI.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"

CButtonUI::CButtonUI(LPDIRECT3DDEVICE9 pGraphicDev, VEHICLE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(eType)
{
}

CButtonUI::CButtonUI(const CButtonUI& rhs)
	: Engine::CGameObject(rhs)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
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

	m_fScaleX = 70.f;
	m_fScaleY = 50.f;
	m_fScaleZ = 0.01f;

	m_fPosX = WINCX * 0.5f;
	m_fPosY = m_fOriginY;
	m_fPosZ = 0.1f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);

	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CButtonUI::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	KeyInput();
	SmoothYMove(fTimeDelta);

	if (OnMouseClick())
	{
		BuyVehicle();
		m_bClicked = false;
	}

	HideText(fTimeDelta);

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
	RenderButton();
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
	CGameObject* pGameObject = static_cast<CGameObject*>(*CTankManager::GetInstance()->GetVehicle());
	Tank_State state = static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Get_TankInfo();

	_vec3 vLength = static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Get_Info() - CGameMode::GetInstance()->m_AllyBasePosition;
	float fLength = D3DXVec3Length(&vLength);


	if (!pGameObject->Get_Dead())
	{
		if (fLength > 250.f)
		{
			CGameMode::GetInstance()->m_bOnSelectButton = false;
		}
	}

	if (Get_DIKeyState_Custom(DIK_O) == KEY_STATE::TAP && fLength <= 250.f
		|| (pGameObject->Get_Dead() && Get_DIKeyState_Custom(DIK_O) == KEY_STATE::TAP))
	{
		if (CGameMode::GetInstance()->m_bOnCreateButton)
		{
			ShowCursor(false);
			CGameMode::GetInstance()->m_bOnCreateButton = false;
		}

		CGameMode::GetInstance()->m_bOnTrigger = true;
		CGameMode::GetInstance()->m_bOnSelectButton = !CGameMode::GetInstance()->m_bOnSelectButton;

		m_fSmoothStart = 0.f;
		static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Set_Rock(CGameMode::GetInstance()->UseMenu());
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

void CButtonUI::RenderButton()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix, OldProjection;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_bPressed ? m_pClickedTexture->Set_Texture(0) : m_pNonClickTexture->Set_Texture(0);

	m_pRcTex->Render_Buffer();

	if (CGameMode::GetInstance()->m_bOnSelectButton)
	{
		_vec2 vPos = { WINCX * 0.5f - 100.f, WINCY * 0.5f - 40.f };
		Engine::Render_Font(L"Font_Retro1", L"차량을 선택하세요", &vPos, D3DCOLOR_ARGB(255, 255, 255, 0));

		wstring strGold = to_wstring(CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY]) + L"  GOLD";
		vPos = { WINCX * 0.5f - (10.f * (strGold.length() * 0.5f)) , WINCY * 0.5f };
		Engine::Render_Font(L"Font_Retro", strGold.c_str(), &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (!hasGold)
		{
			_vec2 vPos = { WINCX * 0.5f - 70.f , WINCY * 0.5f + 40.f };
			Engine::Render_Font(L"Font_Retro", L"골드가 부족합니다.", &vPos, D3DCOLOR_ARGB(255, 230, 230, 230));

			if (m_bHideText)
			{
				hasGold = true;
				m_bHideText = false;
			}
		}
	}

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}

void CButtonUI::BuyVehicle()
{
	switch (m_eType)
	{
	case VEHICLE::HUMVEE:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 200)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 200;
			CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::HUMVEE);
		}
		else
		{
			hasGold = false;
		}
		break;
	case VEHICLE::SMALL_TANK:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 800)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 800;
			CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::SMALL_TANK);
		}
		else
		{
			hasGold = false;
		}
		break;
	case VEHICLE::MIDDLE_TANK:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 1500)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 1500;
			CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::MIDDLE_TANK);
		}
		else
		{
			hasGold = false;
		}
		break;
	case VEHICLE::BIG_TANK:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 2500)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 2500;
			CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::BIG_TANK);
		}
		else
		{
			hasGold = false;
		}
		break;
	case VEHICLE::LONG_TANK:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 2300)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 2300;
			CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::LONG_TANK);
		}
		else
		{
			hasGold = false;
		}
		break;
	default:
		MSG_BOX("BuyVehicle() 인덱스 범위 초과");
		break;
	}

	if (!hasGold)
	{
		return;
	}

	CLayer* pLayer = Engine::Get_Layer(L"GameLogic");
	CGameObject* pGameObject = Engine::Swap_Object(L"GameLogic", L"PlayerVehicle", *CTankManager::GetInstance()->GetVehicle());
	static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Set_Rock(true);

	pGameObject->Set_Dead(false);
}

void CButtonUI::SmoothYMove(const float& fDeltaTime)
{
	m_fSmoothStart += fDeltaTime;

	switch (m_eType)
	{
	case VEHICLE::HUMVEE:
		if (m_fSmoothStart >= 0.4f)
		{
			if (CGameMode::GetInstance()->m_bOnSelectButton)
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fDeltaY, fDeltaTime * 2.f);
			}
			else
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fOriginY, fDeltaTime * 2.f);
			}
		}
		break;
	case VEHICLE::SMALL_TANK:
		if (m_fSmoothStart >= 0.2f)
		{
			if (CGameMode::GetInstance()->m_bOnSelectButton)
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fDeltaY, fDeltaTime * 2.f);
			}
			else
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fOriginY, fDeltaTime * 2.f);
			}
		}
		break;
	case VEHICLE::MIDDLE_TANK:
		if (m_fSmoothStart >= 0.0f)
		{
			if (CGameMode::GetInstance()->m_bOnSelectButton)
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fDeltaY, fDeltaTime * 2.f);
			}
			else
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fOriginY, fDeltaTime * 2.f);
			}
		}
		break;
	case VEHICLE::BIG_TANK:
		if (m_fSmoothStart >= 0.2f)
		{
			if (CGameMode::GetInstance()->m_bOnSelectButton)
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fDeltaY, fDeltaTime * 2.f);
			}
			else
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fOriginY, fDeltaTime * 2.f);
			}
		}
		break;
	case VEHICLE::LONG_TANK:
		if (m_fSmoothStart >= 0.4f)
		{
			if (CGameMode::GetInstance()->m_bOnSelectButton)
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fDeltaY, fDeltaTime * 2.f);
			}
			else
			{
				m_fPosY = Utility::Lerp(m_fPosY, m_fOriginY, fDeltaTime * 2.f);
			}
		}
		break;
	}

	Set_PosX(m_fPosX);
	Set_PosY(m_fPosY);
}

void CButtonUI::HideText(const float& fDeltaTime)
{
	if (!hasGold)
	{
		m_fHideTimer += fDeltaTime;

		if (m_fHideTimer >= 1.5f)
		{
			m_bHideText = true;
			m_fHideTimer = 0.f;
		}
	}
}
