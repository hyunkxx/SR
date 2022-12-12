#include "stdafx.h"
#include "..\Header\Posin_UI.h"
#include "Stage.h"
#include "Tank_01.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
CPosin_UI::CPosin_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPosin_UI::CPosin_UI(const CPosin_UI& rhs)
	: Engine::CGameObject(rhs)
{
}

CPosin_UI::~CPosin_UI()
{
}

HRESULT CPosin_UI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 400.f;
	m_fScaleY = 350.f;
	m_fScaleZ = 1.f;

	m_fPosX = 400.f;
	m_fPosY = 330.f;
	m_fPosZ = 0.01f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}


_int CPosin_UI::Update_Object(const _float & fTimeDelta)
{

	if (Get_DIKeyState_Custom(DIK_F11) == KEY_STATE::TAP)
	{
		m_bPosinOn = !m_bPosinOn;
	}

	__super::Update_Object(fTimeDelta);

	return 0;
}

void CPosin_UI::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	
	Add_RenderGroup(RENDER_UI, this);
	
}

void CPosin_UI::Render_Object(void)
{
	CGameObject* pTankView = Engine::Get_Object(L"Environment", L"TankCamera");
	CGameObject* pStaticView = Engine::Get_Object(L"Environment", L"StaticCamera");
	CGameObject* pAimView = Engine::Get_Object(L"Environment", L"AimCamera");

	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA || Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA || !m_bPosinOn/* ||Engine::Get_Camera_ID() == CAMERA_ID::TOPVIEW_CAMERA || !m_bPosinOn*/)
	{
		m_pTransform->Set_Scale(1.f, 1.f, 1.f);
		m_pTransform->Set_Pos(1.f, 1.f, 1.f);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA || m_bPosinOn)
	{
		if (Get_DIKeyState_Custom(DIK_1) == KEY_STATE::HOLD)
		{
			m_fScaleX += 1.5f;
			m_fScaleY += 1.f;

			if (m_fScaleX >= 520.f) { m_fScaleX = 520.f; }
			if (m_fScaleY >= 430.f) { m_fScaleY = 430.f; }

		}
		if (Get_DIKeyState_Custom(DIK_2) == KEY_STATE::HOLD)
		{
			m_fScaleX -= 1.5f;
			m_fScaleY -= 1.f;

			if (m_fScaleX <= 400.f) { m_fScaleX = 400.f; }
			if (m_fScaleY <= 350.f) { m_fScaleY = 350.f; }
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

}

CPosin_UI * CPosin_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPosin_UI*		pInstance = new CPosin_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Posin_UI Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CPosin_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_Posin_UI_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Posin_UI_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Posin_UI_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Posin_UI_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	return S_OK;
}

void CPosin_UI::Free(void)
{
	CGameObject::Free();
}
