#include "stdafx.h"
#include "..\Header\Player_Chatting.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"
#include "UI_World_Bubble.h"


CPlayer_Chatting::CPlayer_Chatting(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPlayer_Chatting::CPlayer_Chatting(const CPlayer_Chatting& rhs)
	: Engine::CGameObject(rhs)
{
}

CPlayer_Chatting::~CPlayer_Chatting()
{
}

HRESULT CPlayer_Chatting::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 125.f;
	m_fScaleY = 15.f;
	m_fScaleZ = 1.f;

	m_fPosX = (_float)WINCX_HALF - PERCENTX * 3.f;
	m_fPosY = (_float)WINCY_HALF + PERCENTY * 5.f;
	m_fPosZ = 0.01f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CPlayer_Chatting::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	m_szPlayerName = CUI_FontMgr::GetInstance()->Get_PlayerName();

	CGameObject*		pTankView = pTankView = Engine::Get_Object(L"Environment", L"TankCamera");
	CGameObject*		pStaticView = pStaticView = Engine::Get_Object(L"Environment", L"StaticCamera");

	if (Engine::Get_Camera_ID()  ==CAMERA_ID::TANK_CAMERA)
	{
		if (m_bChatting == true)
		{
			Chatting(fTimeDelta);
		}

		if (Get_DIKeyState_Custom(DIK_RETURN) == KEY_STATE::TAP)
		{
			if (m_bChatting == true)
			{
				m_bChatting = false;
			}
			else
			{
				m_bChatting = true;
			}
		}
	}

	return 0;
}

void CPlayer_Chatting::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CPlayer_Chatting::Render_Object(void)
{
	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		if (m_bChatting)
		{
			Render_Font(L"Font_Retro3", m_PlayerChating, &_vec2(m_fPosX - PERCENTX * 15.f, m_fPosY - 5.f), CUI_FontMgr::GetInstance()->Get_Hecks_B());

			m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
			m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
			_matrix	ViewMatrix;
			D3DXMatrixIdentity(&ViewMatrix);

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

			m_pTexture->Set_Texture(0);
			m_pRcTex->Render_Buffer();
		}
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{
		m_pTransform->Set_Scale(1.f, 1.f, 1.f);
		m_pTransform->Set_Pos(1.f, 1.f, 1.f);
	}
}

CPlayer_Chatting * CPlayer_Chatting::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Chatting*		pInstance = new CPlayer_Chatting(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Player Chatting Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CPlayer_Chatting::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Player_Chat_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Chat_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Player_Chat", pComponent });


	return S_OK;
}

void CPlayer_Chatting::Free(void)
{
	CGameObject::Free();
}

void CPlayer_Chatting::Chatting(const _float & fTimeDelta)
{
#pragma region
	if (Get_DIKeyState_Custom(DIK_1) == KEY_STATE::TAP) { m_szInput_Chatting = L"1"; 	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_2) == KEY_STATE::TAP) { m_szInput_Chatting = L"2";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_3) == KEY_STATE::TAP) { m_szInput_Chatting = L"3";		m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_4) == KEY_STATE::TAP) { m_szInput_Chatting = L"4";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_5) == KEY_STATE::TAP) { m_szInput_Chatting = L"5";		m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_6) == KEY_STATE::TAP) { m_szInput_Chatting = L"6";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_7) == KEY_STATE::TAP) { m_szInput_Chatting = L"7";		m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_8) == KEY_STATE::TAP) { m_szInput_Chatting = L"8";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_9) == KEY_STATE::TAP) { m_szInput_Chatting = L"9";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_0) == KEY_STATE::TAP) { m_szInput_Chatting = L"0";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }

	// 채팅창  문자

	if (Get_DIKeyState_Custom(DIK_Q) == KEY_STATE::TAP) { m_szInput_Chatting = L"Q";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_W) == KEY_STATE::TAP) { m_szInput_Chatting = L"W";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_E) == KEY_STATE::TAP) { m_szInput_Chatting = L"E";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_R) == KEY_STATE::TAP) { m_szInput_Chatting = L"R";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_T) == KEY_STATE::TAP) { m_szInput_Chatting = L"T";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_Y) == KEY_STATE::TAP) { m_szInput_Chatting = L"Y";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_U) == KEY_STATE::TAP) { m_szInput_Chatting = L"U";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_I) == KEY_STATE::TAP) { m_szInput_Chatting = L"I";		m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_O) == KEY_STATE::TAP) { m_szInput_Chatting = L"O";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_P) == KEY_STATE::TAP) { m_szInput_Chatting = L"P";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }

	if (Get_DIKeyState_Custom(DIK_A) == KEY_STATE::TAP) { m_szInput_Chatting = L"A";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_S) == KEY_STATE::TAP) { m_szInput_Chatting = L"S";		m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_D) == KEY_STATE::TAP) { m_szInput_Chatting = L"D";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_F) == KEY_STATE::TAP) { m_szInput_Chatting = L"F";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_G) == KEY_STATE::TAP) { m_szInput_Chatting = L"G";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_H) == KEY_STATE::TAP) { m_szInput_Chatting = L"H";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_J) == KEY_STATE::TAP) { m_szInput_Chatting = L"J";		m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_K) == KEY_STATE::TAP) { m_szInput_Chatting = L"K";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_L) == KEY_STATE::TAP) { m_szInput_Chatting = L"L";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }

	if (Get_DIKeyState_Custom(DIK_Z) == KEY_STATE::TAP) { m_szInput_Chatting = L"Z";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_X) == KEY_STATE::TAP) { m_szInput_Chatting = L"X";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_C) == KEY_STATE::TAP) { m_szInput_Chatting = L"C";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_V) == KEY_STATE::TAP) { m_szInput_Chatting = L"V";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_B) == KEY_STATE::TAP) { m_szInput_Chatting = L"B";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_N) == KEY_STATE::TAP) { m_szInput_Chatting = L"N";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_M) == KEY_STATE::TAP) { m_szInput_Chatting = L"M";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_COMMA) == KEY_STATE::TAP) { m_szInput_Chatting = L",";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_PERIOD) == KEY_STATE::TAP) { m_szInput_Chatting = L".";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_SLASH) == KEY_STATE::TAP) { m_szInput_Chatting = L"/";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (Get_DIKeyState_Custom(DIK_SPACE) == KEY_STATE::TAP) { m_szInput_Chatting = L" ";	m_szOutput_Chatting = m_szOutput_Chatting + m_szInput_Chatting;	m_szInput_Chatting = L""; }
	if (m_szOutput_Chatting != L"") { if (Get_DIKeyState_Custom(DIK_BACK) == KEY_STATE::HOLD) { m_szOutput_Chatting.pop_back(); } }

#pragma endregion

	wcscpy_s(m_PlayerChating, m_szOutput_Chatting.c_str());




	if (Get_DIKeyState_Custom(DIK_RETURN) == KEY_STATE::TAP)
	{
		//m_szOutput_Chatting (wstring) 을 폰트 매니저에 보낸다. 폰트 매니저는  문자열을 실시간으로 저장한다.
		// 로그와 버블은 동시에 실시간으로 폰트 매니저의 문자열을 복사_저장 한다.
		// 마지막 최종 형태는 버블은 wstring을 실시간으로 복사해서 저장한다.
		CUI_FontMgr::GetInstance()->SendChatLog(m_szPlayerName, m_szOutput_Chatting);
		//	CUI_FontMgr::GetInstance()->Set_Chatting(m_szOutput_Chatting);
		m_szOutput_Chatting = L"";
	}


}
