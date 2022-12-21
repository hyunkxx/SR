#include "stdafx.h"
#include "..\Header\UI_Start.h"
#include "Stage.h"
#include "Tank_01.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"
#include "GameMode.h"
#include "TankManager.h"
#include "UI_Volume.h"

CUI_Start::CUI_Start(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Start::CUI_Start(const CUI_Start& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Start::~CUI_Start()
{
}

HRESULT CUI_Start::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 400.f;
	m_fScaleY = 300.f;
	m_fScaleZ = 1.f;

	m_fPosX = 400.f;
	m_fPosY = 300.f;
	m_fPosZ = 0.01f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);


	m_fSliderScaleX = 20.f;
	m_fSliderScaleY = 100.f;
	m_fSliderScaleZ = 1.f;

	m_fSliderPosX = 730.f;
	m_fSliderPosY = 100.f;
	m_fSliderPosZ = 0.001f;

	m_fFontPosY = 100.f;


	m_pSliderTransform->Set_Scale(m_fSliderScaleX, m_fSliderScaleY, m_fSliderScaleZ);
	m_pSliderTransform->Set_Pos(m_fSliderPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fSliderPosY, m_fSliderPosZ);

	Ready_Font();

	return S_OK;
}


_int CUI_Start::Update_Object(const _float & fTimeDelta)
{

	Key_input();

	Update_Pos();

	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Start::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);

}

void CUI_Start::Render_Object(void)
{
	if ((Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA) && m_bHelp)
	{
		UI_Render_Font();

		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);


		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();


		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pSliderTransform->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		m_pSliderTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();
	}
}

CUI_Start * CUI_Start::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Start*		pInstance = new CUI_Start(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Start_Ui Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Start::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Start_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Start_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Start", pComponent });


	// Slider
	pComponent = m_pSliderTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Slider_Tex"));
	NULL_CHECK_RETURN(m_pSliderTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Slider_Tex", pComponent });

	pComponent = m_pSliderTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pSliderTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Slider_Start", pComponent });






	return S_OK;
}

void CUI_Start::Free(void)
{
	CGameObject::Free();
}

void CUI_Start::Key_input(void)
{
	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		CGameObject* pVolumeOn = Engine::Get_Object(L"UI", L"Volume_UI");
		if (static_cast<CUI_Volume*>(pVolumeOn)->Get_Volume_Show())
			return;

		if (Get_DIKeyState_Custom(DIK_F7) == KEY_STATE::TAP)
		{
			m_bHelp = !m_bHelp;

			ShowCursor(m_bHelp);
			static_cast<CTankCamera*>(Engine::Get_Camera(L"TankCamera"))->Set_MouseFix(m_bHelp);


			PlaySound_SR(L"F3Sound.wav", UI_BUTTON_SOUND, CUI_Volume::s_fAllSound);

		}

		_long MouseWheel = Get_DIMouseMove(DIMS_Z);

		if ((MouseWheel > 0) || Get_DIKeyState_Custom(DIK_UP) == KEY_STATE::HOLD)
		{
			m_fSliderPosY -= 20.f;

			if (m_fSliderPosY <= 100.f)
			{
				m_fSliderPosY = 100.f;
			}

			m_fFontPosY += 50.f;

			if (m_fFontPosY >= 100.f)
			{
				m_fFontPosY = 100.f;
			}
		}
		if ((MouseWheel < 0) || Get_DIKeyState_Custom(DIK_DOWN) == KEY_STATE::HOLD)
		{
			m_fSliderPosY += 20.f;

			if (m_fSliderPosY >= 500.f)
			{
				m_fSliderPosY = 500.f;
			}

			m_fFontPosY -= 50.f;

			if (m_fFontPosY <= -1100.f)
			{
				m_fFontPosY = -1100.f;
			}


		}
	}
}

void CUI_Start::Update_Pos(void)
{
	if (!m_bHelp)
		return;

	m_pSliderTransform->Set_Pos(m_fSliderPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fSliderPosY, m_fSliderPosZ);

}

void CUI_Start::Ready_Font(void)
{

	m_szDescription[NUM_HELP] = L"도움말";

	// UI
	m_szDescription[NUM_UI] = L"UI";
	m_szDescription[NUM_P] = L"P";
	m_szDescription[NUM_O] = L"O";
	m_szDescription[NUM_F3] = L"F3";
	m_szDescription[NUM_F7] = L"F7";
	m_szDescription[NUM_CE] = L"C";
	m_szDescription[NUM_T] = L"T";
	m_szDescription[NUM_BLANK1] = L"";

	// Player -- Normal Tank
	m_szDescription[NUM_BASE] = L"기본 조작";
	m_szDescription[NUM_MOVE] = L"W , S , A , D";
	m_szDescription[NUM_LB] = L"Mouse Left";
	m_szDescription[NUM_M] = L"M";
	m_szDescription[NUM_G] = L"G";
	m_szDescription[NUM_V] = L"V";
	m_szDescription[NUM_1] = L"->1";
	m_szDescription[NUM_2] = L"->2";
	m_szDescription[NUM_BLANK2] = L"";

	// Skill
	m_szDescription[NUM_SKILL] = L"SKILL";
	m_szDescription[NUM_3] = L"3";
	m_szDescription[NUM_4] = L"4";
	m_szDescription[NUM_5] = L"5";
	m_szDescription[NUM_BLANK3] = L"";

	// Player -- Long Tank
	m_szDescription[NUM_M55] = L"M_55 Skill";
	m_szDescription[NUM_Q] = L"Q";
	m_szDescription[NUM_BLANK4] = L"";

	// Player -- 아파치
	m_szDescription[NUM_AH_64A] = L"AH_64A (아파치 헬기) 조종";
	m_szDescription[NUM_MOVE2] = L"W , S , A , D";
	m_szDescription[NUM_LB2] = L"Mouse Left";
	m_szDescription[NUM_RB] = L"Mouse Right";
	m_szDescription[NUM_LC] = L"Left Control Key";






	m_szDescription_Field[NUM_HELP] = L"";

	// UI
	m_szDescription_Field[NUM_UI] = L"";
	m_szDescription_Field[NUM_P] = L"아군 탱크 생성";
	m_szDescription_Field[NUM_O] = L"탱크 변경";
	m_szDescription_Field[NUM_F3] = L"사운드 조절 ON / OFF";
	m_szDescription_Field[NUM_F7] = L"도움말 ON / OFF";
	m_szDescription_Field[NUM_CE] = L"Emoticon";
	m_szDescription_Field[NUM_T] = L"타겟점 UI 변경";
	m_szDescription_Field[NUM_BLANK1] = L"";

	// Player -- Normal Tank
	m_szDescription_Field[NUM_BASE] = L"";
	m_szDescription_Field[NUM_MOVE] = L"전진, 후진, 방향 전환";
	m_szDescription_Field[NUM_LB] = L"포탄 발사";
	m_szDescription_Field[NUM_M] = L"플레이어 사망 모션";
	m_szDescription_Field[NUM_G] = L"탱크 시동";
	m_szDescription_Field[NUM_V] = L"탱크 조준경 ON / OFF";
	m_szDescription_Field[NUM_1] = L"조준경 확대";
	m_szDescription_Field[NUM_2] = L"조준경 축소";
	m_szDescription_Field[NUM_BLANK2] = L"";

	// Skill
	m_szDescription_Field[NUM_SKILL] = L"";
	m_szDescription_Field[NUM_3] = L"스킬 1 : 비행기 폭격";
	m_szDescription_Field[NUM_4] = L"스킬 2 : 함선 폭격";
	m_szDescription_Field[NUM_5] = L"스킬 3 : 아파치 헬기 호출";
	m_szDescription_Field[NUM_BLANK3] = L"";

	// Player -- Long Tank
	m_szDescription_Field[NUM_M55] = L"";
	m_szDescription_Field[NUM_Q] = L"연막탄 발사";
	m_szDescription_Field[NUM_BLANK4] = L"";

	// Player -- 아파치
	m_szDescription_Field[NUM_AH_64A] = L"";
	m_szDescription_Field[NUM_MOVE2] = L"방향 이동";
	m_szDescription_Field[NUM_LB2] = L"헬기 기관총";
	m_szDescription_Field[NUM_RB] = L"헬기 1 인칭 시점 ON / OFF";
	m_szDescription_Field[NUM_LC] = L"전, 후진 고정";

}

void CUI_Start::UI_Render_Font(void)
{

	for (_int i = 0; i < NUM_END; ++i)
	{
		_float fY = (m_fFontPosY - PERCENTY * 5.f) + (i * 50.f);

		if (i == NUM_HELP)
		{
			Render_Font(L"Font_Retro1", (m_szDescription[i]).c_str(), &_vec2(200, fY - 20.f), D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
			continue;
		}
		else if (i == NUM_UI || i == NUM_BASE || i == NUM_SKILL || i == NUM_M55 || i == NUM_AH_64A)
		{
			Render_Font(L"Font_Retro1", (m_szDescription[i]).c_str(), &_vec2(150.f, fY), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			continue;
		}
		else if (i == NUM_1 || i == NUM_2)
		{
			Render_Font(L"Font_Retro1", (m_szDescription[i]).c_str(), &_vec2(100.f, fY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
			Render_Font(L"Font_Retro1", (m_szDescription_Field[i]).c_str(), &_vec2(300.f, fY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
			continue;
		}

		Render_Font(L"Font_Retro1", (m_szDescription[i]).c_str(), &_vec2(50.f, fY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		Render_Font(L"Font_Retro1", (m_szDescription_Field[i]).c_str(), &_vec2(300.f, fY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	}
}
