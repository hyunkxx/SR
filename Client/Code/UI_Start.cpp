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
	_int iPlayTime = CUI_FontMgr::GetInstance()->Get_PlayTime();

	if (iPlayTime >= 180)
	{
		m_bHelp = true;
	}
	if (iPlayTime == 179)
	{
		m_bHelp = false;
	}

	if (Get_DIKeyState_Custom(DIK_F1) == KEY_STATE::TAP)
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

		HelpWinFont1 = L"G: ��ũ �õ� ON	 \n\nW S A D : �̵� \n\n��Ŭ��: ��ź ���� \n\nV : ���� ��ȯ\n\nK : ���� ���� ���� \n\nENTER : ä��		\n\nENTER + NUM : �� ��ũ ����";
															
		HelpWinFont2 = L"���� 1  : �þ� �� ��	 \n\n���� 2  : �þ� �� �ƿ�	 \n\nG : (������) ����ź \n\nC : �̸�Ƽ�� \n\nF1 : ����		\n\nF3 : ���� �ɼ�";
			
		/*	1��Ī ī�޶� ���� 1�� 2������ �þ� ���� �� �ƿ�
			KŰ : ���ݱ� )��� ī�޶�� �����)
				 wsad �̵� ��Ŭ ����

			Long_tank ���� GŰ : ����ź
			����Ű�� ������ �ִ� �� ���ƿ�

			0% ����, 58.82% ��� �� 77.65% �Ķ�
			D3DXCOLOR(0.f, 0.5882f, 0.7765f, 1.f) ����

			92.94% ����, 67.45% ��� �� 69.41% �Ķ�
			D3DXCOLOR(0.9294f, 0.6745f, 0.6941f, 1.f) ��¥ ������

			40% ����, 0% ��� �� 60% �Ķ�		����

			0% ����, 20.39% ��� �� 34.51% �Ķ�.			��� ���� ���γ�
*/


		Render_Font(L"Font_Retro1", (HelpWinFont1).c_str(), &_vec2(50.f, 100.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		
		Render_Font(L"Font_Retro1", (HelpWinFont2).c_str(), &_vec2(400.f, 100.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

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

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_Posin_UI_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Posin_UI_RcTex", pComponent });

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
