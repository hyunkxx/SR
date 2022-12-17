#include "stdafx.h"
#include "..\Header\DirButton.h"

#include "Export_Function.h"
#include "CameraMgr.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"
#include "BottomDirAlly.h"
#include "BottomDirEnermy.h"
#include "Default_Ally.h"
#include "Default_Enermy.h"

CDirButton::CDirButton(LPDIRECT3DDEVICE9 pGraphicDev, DIR eType)
	: Engine::CGameObject(pGraphicDev)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(eType)
{
}

CDirButton::CDirButton(const CDirButton& rhs)
	: Engine::CGameObject(rhs)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(rhs.m_eType)
{
}


CDirButton::~CDirButton()
{
}

CDirButton* CDirButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, DIR eType)
{
	CDirButton*	pInstance = new CDirButton(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CDirButton Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CDirButton::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 60.f;
	m_fScaleY = 40.f;
	m_fScaleZ = 0.01f;

	m_fPosX = WINCX - m_fScaleX;
	m_fPosY = m_fOriginY;
	m_fPosZ = 0.1f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CDirButton::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	KeyInput();
	SmoothYMove(fTimeDelta);

	if (OnMouseClick())
	{
		CreateVehicle();
		m_bClicked = false;
	}

	if (!hasGold || m_bMaxCount)
	{
		m_fHideTimer += fTimeDelta;

		if (m_fHideTimer >= 1.5f)
		{
			m_bHideText = true;
			m_fHideTimer = 0.f;
		}
	}

	UpdateTransform();

	return 0;
}

void CDirButton::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CDirButton::Render_Object(void)
{
	RenderButton();
}

void CDirButton::Free(void)
{
	__super::Free();
}

HRESULT CDirButton::Add_Component(void)
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
	case DIR::LEFT:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_left_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_left_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case DIR::RIGHT:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_right_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_right_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	default:
		MSG_BOX("CDirButton 타입범위 초과");
		break;
	}

	return S_OK;
}

void CDirButton::KeyInput()
{
	CGameObject* pGameObject = static_cast<CGameObject*>(*CTankManager::GetInstance()->GetVehicle());
	Tank_State state = static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Get_TankInfo();

	_vec3 vLength = static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Get_Info() - CGameMode::GetInstance()->m_AllyBasePosition;
	float fLength = D3DXVec3Length(&vLength);

	if (Get_DIKeyState_Custom(DIK_P) == KEY_STATE::TAP)
	{
		Engine::StopSound(PAPER_SOUND);
		Engine::PlaySound_SR(L"Paper.wav", PAPER_SOUND, 1.f);

		if (CGameMode::GetInstance()->m_bOnSelectButton)
		{
			ShowCursor(false);
			CGameMode::GetInstance()->m_bOnSelectButton = false;
		}

		CGameMode::GetInstance()->m_bOnTrigger = true;
		CGameMode::GetInstance()->m_bOnCreateButton = !CGameMode::GetInstance()->m_bOnCreateButton;


		m_fSmoothStart = 0.f;
		static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Set_Rock(CGameMode::GetInstance()->UseMenu());
	}
}

bool CDirButton::OnMouseClick()
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

void CDirButton::UpdateTransform()
{
	if (m_bUpdateTransform)
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
		m_bUpdateTransform = false;
	}
}

void CDirButton::RenderButton()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix, OldProjection;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_bPressed ? m_pClickedTexture->Set_Texture(0) : m_pNonClickTexture->Set_Texture(0);

	m_pRcTex->Render_Buffer();

	if (CGameMode::GetInstance()->m_bOnCreateButton)
	{
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

		if (m_bMaxCount)
		{
			_vec2 vPos = { WINCX * 0.5f - 120.f , WINCY * 0.5f + 40.f };
			Engine::Render_Font(L"Font_Retro", L"차량을 더이상 생산할 수 없습니다.", &vPos, D3DCOLOR_ARGB(255, 230, 230, 230));

			if (m_bHideText)
			{
				m_bMaxCount = false;
				m_bHideText = false;
			}
		}
	}

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}

void CDirButton::CreateVehicle()
{
	EData eData = CGameMode::GetInstance()->eData;

	if (eData.TankType == (UINT)VEHICLE::MAX)
		return;

	if (CGameMode::GetInstance()->m_nAllyCount == CGameMode::GetInstance()->m_nUnitMaxCount)
	{
		m_bMaxCount = true;
		return;
	}
	else
	{
		m_bMaxCount = false;
	}

	switch (eData.TankType)
	{
	case (UINT)VEHICLE::HUMVEE:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 200)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 200;
		}
		else
		{
			hasGold = false;
		}
		break;
	case (UINT)VEHICLE::SMALL_TANK:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 800)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 800;
		}
		else
		{
			hasGold = false;
		}
		break;
	case (UINT)VEHICLE::MIDDLE_TANK:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 1500)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 1500;
		}
		else
		{
			hasGold = false;
		}
		break;
	case (UINT)VEHICLE::BIG_TANK:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 2500)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 2500;
		}
		else
		{
			hasGold = false;
		}
		break;
	case (UINT)VEHICLE::LONG_TANK:
		if (CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] >= 2300)
		{
			hasGold = true;
			CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY] -= 2300;
		}
		else
		{
			hasGold = false;
		}
		break;
	default:
		break;
	}

	if (!hasGold)
	{
		ZeroMemory(&CGameMode::GetInstance()->eData, sizeof(EData));
		return;
	}

	EData da = CGameMode::GetInstance()->eData;

	switch (m_eType)
	{
	case DIR::LEFT:
	{
		CGameObject* pEnermy = CDefault_Ally::Create(m_pGraphicDev, &eData);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ALLY);
		break;
	}
	case DIR::RIGHT:
	{
		CGameObject* pEnermy = CBottomDirAlly::Create(m_pGraphicDev, &eData);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDALLY);
		break;
	}
	default:
		MSG_BOX("DirButton::CreateVehicle() 인덱스 범위 초과");
		break;
	}
}

void CDirButton::SmoothYMove(const float & fDeltaTime)
{
	m_fSmoothStart += fDeltaTime;

	if (CGameMode::GetInstance()->m_bOnCreateButton)
	{
		m_fPosY = Utility::Lerp(m_fPosY, m_fDeltaY, fDeltaTime * 3.f);
	}
	else
	{
		m_fPosY = Utility::Lerp(m_fPosY, m_fOriginY, fDeltaTime * 3.f);
	}

	Set_PosX(m_fPosX);
	Set_PosY(m_fPosY);
}
