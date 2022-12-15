#include "stdafx.h"
#include "..\Header\AICreateButton.h"

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

CAICreateButton::CAICreateButton(LPDIRECT3DDEVICE9 pGraphicDev, VEHICLE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(eType)
{
}

CAICreateButton::CAICreateButton(const CAICreateButton& rhs)
	: Engine::CGameObject(rhs)
	, m_bShow(false)
	, m_bClicked(false)
	, m_bPressed(false)
	, m_bUpdateTransform(true)
	, m_eType(rhs.m_eType)
{
}


CAICreateButton::~CAICreateButton()
{
}

CAICreateButton* CAICreateButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, VEHICLE eType)
{
	CAICreateButton*	pInstance = new CAICreateButton(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CAICreateButton Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CAICreateButton::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 80.f;
	m_fScaleY = 50.f;
	m_fScaleZ = 0.01f;

	m_fPosX = WINCX - m_fScaleX;
	m_fPosY = m_fOriginY;
	m_fPosZ = 0.1f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CAICreateButton::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	KeyInput();
	SmoothYMove(fTimeDelta);

	if (OnMouseClick())
	{
		SelectVehicle();
		m_bClicked = false;
	}

	UpdateTransform();

	return 0;
}

void CAICreateButton::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CAICreateButton::Render_Object(void)
{
	RenderButton();
}

void CAICreateButton::Free(void)
{
	__super::Free();
}

HRESULT CAICreateButton::Add_Component(void)
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
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_humvee_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_humvee_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case Engine::VEHICLE::SMALL_TANK:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_small_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_small_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case Engine::VEHICLE::MIDDLE_TANK:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_middle_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_middle_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case Engine::VEHICLE::BIG_TANK:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_big_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_big_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	case Engine::VEHICLE::LONG_TANK:
		pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_long_btn"));
		NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

		pComponent = m_pClickedTexture = static_cast<CTexture*>(Clone_Prototype(L"ai_long_btn_click"));
		NULL_CHECK_RETURN(m_pClickedTexture, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_click", pComponent });
		break;
	default:
		MSG_BOX("CAICreateButton 타입범위 초과");
		break;
	}

	return S_OK;
}

void CAICreateButton::KeyInput()
{
	CGameObject* pGameObject = static_cast<CGameObject*>(*CTankManager::GetInstance()->GetVehicle());
	Tank_State state = static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Get_TankInfo();

	_vec3 vLength = static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Get_Info() - CGameMode::GetInstance()->m_AllyBasePosition;
	float fLength = D3DXVec3Length(&vLength);

	if (Get_DIKeyState_Custom(DIK_P) == KEY_STATE::TAP)
	{
		if (CGameMode::GetInstance()->m_bOnSelectButton)
		{
			CGameMode::GetInstance()->m_bOnSelectButton = false;
		}

		CGameMode::GetInstance()->m_bOnCreateButton = !CGameMode::GetInstance()->m_bOnCreateButton;

		m_fSmoothStart = 0.f;
		static_cast<CTankSet*>(*CTankManager::GetInstance()->GetVehicle())->Set_Rock(CGameMode::GetInstance()->UseMenu());
	}
}

bool CAICreateButton::OnMouseClick()
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

void CAICreateButton::UpdateTransform()
{
	if (m_bUpdateTransform)
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
		m_bUpdateTransform = false;
	}
}

void CAICreateButton::RenderButton()
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
		_vec2 vPos = { WINCX * 0.5f - 100.f, WINCY * 0.5f - 40.f };
		Engine::Render_Font(L"Font_Retro1", L"차량을 선택하세요", &vPos, D3DCOLOR_ARGB(255, 255, 255, 0));

		wstring strGold = to_wstring(CGameMode::GetInstance()->m_nGold[(UINT)CGameMode::TYPE::ALLY]) + L"  GOLD";
		vPos = { WINCX * 0.5f - (10.f * (strGold.length() * 0.5f)) , WINCY * 0.5f };
		Engine::Render_Font(L"Font_Retro", strGold.c_str(), &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}

void CAICreateButton::SelectVehicle()
{
	//	CGameObject* pEnermy = CBottomDirAlly::Create(m_pGraphicDev, &eData);
	//	Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDALLY);

	EData eData;

	switch (m_eType)
	{
	case VEHICLE::HUMVEE:
		eData.TankType = TANKTYPE::HUMVEE;
		eData.eID = OBJID::OBJID_BDALLY;
		eData.vPos = { (float)(rand() % 150), 0.f, (float)(rand() % 150) };
		break;
	case VEHICLE::SMALL_TANK:
		eData.TankType = TANKTYPE::SMALL_TANK;
		eData.eID = OBJID::OBJID_BDALLY;
		eData.vPos = { (float)(rand() % 150), 0.f, (float)(rand() % 150) };
		break;
	case VEHICLE::MIDDLE_TANK:
		eData.TankType = TANKTYPE::MIDDLE_TANK;
		eData.eID = OBJID::OBJID_BDALLY;
		eData.vPos = { (float)(rand() % 150), 0.f, (float)(rand() % 150) };
		break;
	case VEHICLE::BIG_TANK:
		eData.TankType = TANKTYPE::BIG_TANK;
		eData.eID = OBJID::OBJID_BDALLY;
		eData.vPos = { (float)(rand() % 150), 0.f, (float)(rand() % 150) };
		break;
	case VEHICLE::LONG_TANK:
		eData.TankType = TANKTYPE::LONG_TANK;
		eData.eID = OBJID::OBJID_BDALLY;
		eData.vPos = { (float)(rand() % 150), 0.f, (float)(rand() % 150) };
		break;
	default:
		MSG_BOX("BuyVehicle() 인덱스 범위 초과");
		break;
	}

	if (!hasGold)
		return;

	CGameMode::GetInstance()->eData = eData;
}

void CAICreateButton::SmoothYMove(const float & fDeltaTime)
{
	m_fSmoothStart += fDeltaTime;

	switch (m_eType)
	{
	case VEHICLE::HUMVEE:
		if (m_fSmoothStart >= 0.4f)
		{
			if (CGameMode::GetInstance()->m_bOnCreateButton)
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
			if (CGameMode::GetInstance()->m_bOnCreateButton)
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
			if (CGameMode::GetInstance()->m_bOnCreateButton)
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
			if (CGameMode::GetInstance()->m_bOnCreateButton)
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
			if (CGameMode::GetInstance()->m_bOnCreateButton)
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
