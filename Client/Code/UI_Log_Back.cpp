#include "stdafx.h"
#include "..\Header\UI_Log_Back.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"


CUI_Log_Back::CUI_Log_Back(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Log_Back::CUI_Log_Back(const CUI_Log_Back& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Log_Back::~CUI_Log_Back()
{
}

HRESULT CUI_Log_Back::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 125.f;
	m_fScaleY = 75.f;
	m_fScaleZ = 1.f;

	m_fPosX = 125.f;
	m_fPosY = 540.f;
	m_fPosZ = 0.01f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);



	return S_OK;
}

_int CUI_Log_Back::Update_Object(const _float & fTimeDelta)
{

	// �÷��� Ÿ��
	m_iPlayerTime = CUI_FontMgr::GetInstance()->Get_PlayTime();

	UpdateChatting();

	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Log_Back::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CUI_Log_Back::Render_Object(void)
{
	CGameObject* pTankView = Engine::Get_Object(L"Environment", L"TankCamera");
	CGameObject* pStaticView = Engine::Get_Object(L"Environment", L"StaticCamera");
	CGameObject* pAimView = Engine::Get_Object(L"Environment", L"AimCamera");

	if ((static_cast<CTankCamera*>(pTankView)->Get_CameraOn()) || static_cast<CStaticCamera*>(pStaticView)->Get_CameraOn())
	{

		_float LogChat_PosX = m_fPosX - PERCENTX * 15.f;
		_float LogChat_PosY = m_fPosY - PERCENTY * 2.5f;

		for (_int i = 0; i < 5; ++i)
		{
			Render_Font(L"Font_Sitka1_1", m_szChattingLog[i].c_str(), &_vec2(LogChat_PosX, LogChat_PosY + (i * 15.f)), CUI_FontMgr::GetInstance()->Get_Hecks_B());
		}


		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	else if (static_cast<CAimCamera*>(pAimView)->Get_CameraOn())
	{
		m_pTransform->Set_Scale(1.f, 1.f, 1.f);
		m_pTransform->Set_Pos(1.f, 1.f, 1.f);
	}
}

CUI_Log_Back * CUI_Log_Back::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Log_Back*		pInstance = new CUI_Log_Back(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("World Chatting Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Log_Back::UpdateChatting(void)
{
	if (m_iPlayerTime == 182)
	{
		// �α�â���� ���� ����~
		m_szChattingLog[0] = L"�ý��� : LOG .. Setting .. Loading";
		m_szChattingLog[1] = L"����Ű Ȯ��";
		m_szChattingLog[2] = L"�ڵ� SAVE _ OFF";
		m_szChattingLog[3] = L"ȯ���մϴ�. �������";
		m_szChattingLog[4] = L"���� 1";


		/*    �ٸ� Ŭ�������� �� ���										��ü �̸�						���ϴ� ���ڿ�
		CUI_FontMgr::GetInstance()->SendChatLog(wstring(L"�ý���"), wstring(L"LOG .. Setting .. Loading"));
		CUI_FontMgr::GetInstance()->SendChatLog(wstring(L"�ý���"), wstring(L"����Ű Ȯ��"));
		CUI_FontMgr::GetInstance()->SendChatLog(wstring(L"�ý���"), wstring(L"�ڵ� SAVE _ OFF"));
		CUI_FontMgr::GetInstance()->SendChatLog(wstring(L"�ý���"), wstring(L"ȯ���մϴ�. �������"));
		CUI_FontMgr::GetInstance()->SendChatLog(wstring(L"�ý���"), wstring(L"������ ���̰� ��� �����ʽÿ�."));
		*/
	}

	if ((m_szChattingLog[4] != CUI_FontMgr::GetInstance()->Get_Chat_Log()))
	{
		m_szChattingLog[0] = m_szChattingLog[1];
		m_szChattingLog[1] = m_szChattingLog[2];
		m_szChattingLog[2] = m_szChattingLog[3];
		m_szChattingLog[3] = m_szChattingLog[4];
		m_szChattingLog[4] = CUI_FontMgr::GetInstance()->Get_Chat_Log();
	}
}

HRESULT CUI_Log_Back::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_Log_Back_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Log_Back_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Log_Back_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Log_Back_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform7", pComponent });


	return S_OK;
}

void CUI_Log_Back::Free(void)
{
	CGameObject::Free();
}
