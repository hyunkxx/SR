#include "stdafx.h"
#include "..\Header\WarningUI.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"

CWarningUI::CWarningUI(LPDIRECT3DDEVICE9 pGraphicDev, MODE eMode)
	: Engine::CGameObject(pGraphicDev)
	, m_eMode(eMode)
{
}

CWarningUI::CWarningUI(const CWarningUI& rhs)
	: Engine::CGameObject(rhs)
	, m_eMode(rhs.m_eMode)
{
}


CWarningUI::~CWarningUI()
{
}

CWarningUI* CWarningUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, MODE eMode)
{
	CWarningUI*	pInstance = new CWarningUI(pGraphicDev, eMode);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CWarningUI Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CWarningUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 200.f;
	m_fScaleY = 100.f;
	m_fScaleZ = 0.01f;

	m_fPosX = WINCX * 0.5f;
	m_fPosY = WINCY * 0.5f - 70.f;
	m_fPosZ = 0.0f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CWarningUI::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	m_pPlayer = Engine::Get_Object(L"GameLogic", L"PlayerVehicle");

	if (m_pPlayer->Get_Dead())
		return 0;

	_vec3 vPos = static_cast<CTankSet*>(m_pPlayer)->Get_Info();

	if (vPos.x <= 0.f || vPos.x >= 630 || vPos.z <= 0.f || vPos.z >= 630)
	{
		m_bTrigger = true;
	}
	else
	{
		StopSound(POSIN_ZOOM);
		m_bTrigger = false;
	}

	if (m_bTrigger)
	{
		Engine::PlaySound_SR(L"warning.mp3", POSIN_ZOOM, 0.5f);

		m_fTimer += fTimeDelta;
		m_fHideTimer += fTimeDelta;
		if (m_fTimer >= m_fTimeOut)
		{
			m_fTimer = 0.f;
			m_bTrigger = false;
			static_cast<CTankSet*>(m_pPlayer)->Set_Dead(true);
		}

		if (m_fHideTimer >= m_fHideTimeOut)
		{
			m_bHide = !m_bHide;
			m_fHideTimer = 0.f;
		}
	}
	else
	{
		m_fTimer = 0.f;
		m_fHideTimer = 0.f;
		m_bHide = false;
		StopSound(POSIN_ZOOM);
	}

	return 0;
}

void CWarningUI::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CWarningUI::Render_Object(void)
{
	if (!m_bTrigger)
		return;

	wstring strWarning = L"군사지역을 벗어났습니다. 전장으로 복귀하세요";
	_vec2 vPos = { float((WINCX * 0.5f) - (15 * (strWarning.length() * 0.5f))), 360.f };
	Engine::Render_Font(L"Font_Retro", strWarning.c_str(), &vPos, D3DCOLOR_RGBA(255, 0, 0, 255));
	wstring strSec = to_wstring((int)(10 - m_fTimer)) + L"초 후 차량이 파괴 됩니다.";
	vPos = { float((WINCX * 0.5f) - (15 * (strSec.length() * 0.5f))), 400.f };
	Engine::Render_Font(L"Font_Retro", strSec.c_str(), &vPos, D3DCOLOR_RGBA(255, 255, 0, 255));

	if (!m_bHide)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix, OldProjection, OldView;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTexture->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}

void CWarningUI::Free(void)
{
	__super::Free();
}

HRESULT CWarningUI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"rect_texture", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"warning"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"texture", pComponent });


	return S_OK;
}