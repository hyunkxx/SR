#include "stdafx.h"
#include "..\Header\Aim_UI.h"

#include  "Export_Function.h"

CAim_UI::CAim_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CAim_UI::CAim_UI(const CAim_UI & rhs)
	: CGameObject(rhs)
{
}

CAim_UI::~CAim_UI()
{
}

HRESULT CAim_UI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	/* 직교투영행렬을 만든다. */

	/* 윈도우좌표 기준의 위치. */
	m_fSizeX = 20.f;
	m_fSizeY = 20.f;
	m_fX = 400.f;
	m_fY = 300.f;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.0f);

	m_bTexture[0] = true;
	m_bTexture[1] = false;
	m_bTexture[2] = false;
	return S_OK;
}

_int CAim_UI::Update_Object(const _float & fTimeDelta)
{
	Key_Input();
	__super::Update_Object(fTimeDelta);

	return OBJ_NOEVENT;
}

void CAim_UI::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CAim_UI::Render_Object(void)
{
	_matrix OldViewMatrix, OldProjMatrix;


	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	_matrix			ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	if(m_bTexture[0])
		m_pTextureCom_0->Set_Texture(0);
	else if (m_bTexture[1])
		m_pTextureCom_1->Set_Texture(0);
	else if (m_bTexture[2])
		m_pTextureCom_2->Set_Texture(0);

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
}

HRESULT CAim_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom_0 = dynamic_cast<CTexture*>(Clone_Prototype(L"target_0"));
	NULL_CHECK_RETURN(m_pTextureCom_0, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"target_0", pComponent });

	pComponent = m_pTextureCom_1 = dynamic_cast<CTexture*>(Clone_Prototype(L"target_1"));
	NULL_CHECK_RETURN(m_pTextureCom_1, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"target_1", pComponent });

	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Clone_Prototype(L"target_2"));
	NULL_CHECK_RETURN(m_pTextureCom_2, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"target_2", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	
	return S_OK;
}

void CAim_UI::Key_Input(void)
{
	if (Engine::Get_DIKeyState_Custom(DIK_T) == KEY_STATE::TAP)
	{
		if (m_bTexture[0])
		{
			m_bTexture[0] = false;
			m_bTexture[1] = true;
		}
		else if (m_bTexture[1])
		{
			m_bTexture[1] = false;
			m_bTexture[2] = true;
		}
		else if (m_bTexture[2])
		{
			m_bTexture[2] = false;
			m_bTexture[0] = true;
		}
	}
}

CAim_UI * CAim_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{

	CAim_UI*		pInstance = new CAim_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CAim_UI Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAim_UI::Free(void)
{
	__super::Free();
}
