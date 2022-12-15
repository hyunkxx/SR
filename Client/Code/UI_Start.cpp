#include "stdafx.h"
#include "..\Header\UI_Start.h"
#include "Stage.h"
#include "Tank_01.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"

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

	return S_OK;
}


_int CUI_Start::Update_Object(const _float & fTimeDelta)
{
	if (Get_DIKeyState_Custom(DIK_F7) == KEY_STATE::TAP)
	{
		m_bHelp = !m_bHelp;
	}

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
	if ((Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)&& m_bHelp)
	{
		wstring HelpWinFont1 , HelpWinFont2;

		HelpWinFont1 = L"G: 탱크 시동 ON	 \n\nW S A D : 이동 \n\n좌클릭: 포탄 공격 \n\nV : 시점 변환\n\nK : 공중 폭격 지원 \n\nENTER : 채팅		\n\nENTER + NUM : 적 탱크 숫자\n\n";
															
		HelpWinFont2 = L"숫자 1  : 시야 줌 인	 \n\n숫자 2  : 시야 줌 아웃	 \n\nG : (자주포) 연막탄 \n\nC : 이모티콘 \n\nJ : 함선 폭격 지원\n\nF1 : 도움말\n\nF3 : 볼륨 옵션";
			
		// 임시 설정 -> 삭제 ( 보여줄 땐, 밑에 걸로)
		wstring HelpWinFont3{ L" 임시 설정\nF11 포신UI 제거\n험비,롱탱 : 죽는키->m" };
		Render_Font(L"Font_Retro", (HelpWinFont1).c_str(), &_vec2(25.f, 25.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		Render_Font(L"Font_Retro", (HelpWinFont2).c_str(), &_vec2(400.f, 25.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		Render_Font(L"Font_Retro", (HelpWinFont3).c_str(), &_vec2(25, 400.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

		// 나중에 고정 될 원본 위치
	/*	Render_Font(L"Font_Retro1", (HelpWinFont1).c_str(), &_vec2(50.f, 100.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		
		Render_Font(L"Font_Retro1", (HelpWinFont2).c_str(), &_vec2(400.f, 100.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));*/

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
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


	return S_OK;
}

void CUI_Start::Free(void)
{
	CGameObject::Free();
}
