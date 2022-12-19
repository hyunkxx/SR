#include "stdafx.h"
#include "..\Header\BaseUI.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"

CBaseUI::CBaseUI(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_bUpdateTransform(true)
	, m_eType(eType)
{
}

CBaseUI::CBaseUI(const CBaseUI& rhs)
	: Engine::CGameObject(rhs)
	, m_bUpdateTransform(true)
	, m_eType(rhs.m_eType)
{
}


CBaseUI::~CBaseUI()
{
}

CBaseUI* CBaseUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType)
{
	CBaseUI*	pInstance = new CBaseUI(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CBaseUI Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CBaseUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 170.f;
	m_fScaleY = 32.f;
	m_fScaleZ = 0.01f;

	m_fPosX = WINCX * 0.5f;
	m_fPosY = 20.f;
	m_fPosZ = 0.1f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);

	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, 0.1f);

	return S_OK;
}

_int CBaseUI::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	switch (m_eType)
	{
	case CBaseUI::TYPE::ALLY:
	{
		curPer = (CGameMode::GetInstance()->m_fBaseCurHP[(UINT)CGameMode::TYPE::ALLY] / CGameMode::GetInstance()->m_fBaseMaxHP[(UINT)CGameMode::TYPE::ALLY]) * 200.f;

		m_fScaleX = curPer;
		_float fX = m_fScaleX - 200.f;
		m_fPosX = 200.f - fX;

		if (curPer <= 0.f)
		{
			CGameMode::GetInstance()->m_eGameResult = CGameMode::GAME_RESULT::LOSE;
			CGameMode::GetInstance()->m_bGameEnd = true;
		}

		m_bUpdateTransform = true;
		break;
	}
	case CBaseUI::TYPE::ENEMY:
	{
		curPer = (CGameMode::GetInstance()->m_fBaseCurHP[(UINT)CGameMode::TYPE::ENEMY] / CGameMode::GetInstance()->m_fBaseMaxHP[(UINT)CGameMode::TYPE::ENEMY]) * 200.f;
		m_fScaleX = curPer;
		_float fX = m_fScaleX - 200.f;
		m_fPosX = 600.f + fX;

		if (curPer <= 0.f)
		{
			CGameMode::GetInstance()->m_eGameResult = CGameMode::GAME_RESULT::WIN;
			CGameMode::GetInstance()->m_bGameEnd = true;
		}

		m_bUpdateTransform = true;
		break;
	}
	}

	UpdateTransform();
	Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CBaseUI::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();


}

void CBaseUI::Render_Object(void)
{
	RenderUI();
}

void CBaseUI::Free(void)
{
	__super::Free();
}

HRESULT CBaseUI::Add_Component(void)
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
	case TYPE::ALLY:
		pComponent = m_pBack = static_cast<CTexture*>(Clone_Prototype(L"base_back"));
		NULL_CHECK_RETURN(m_pBack, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_back", pComponent });

		pComponent = m_pFront = static_cast<CTexture*>(Clone_Prototype(L"base_ally_front"));
		NULL_CHECK_RETURN(m_pFront, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_front", pComponent });
		break;
	case TYPE::ENEMY:
		pComponent = m_pBack = static_cast<CTexture*>(Clone_Prototype(L"base_back"));
		NULL_CHECK_RETURN(m_pBack, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_back", pComponent });

		pComponent = m_pFront = static_cast<CTexture*>(Clone_Prototype(L"base_enemy_front"));
		NULL_CHECK_RETURN(m_pFront, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"texture_front", pComponent });
		break;
	default:
		MSG_BOX("CBaseUI 타입범위 초과");
		break;
	}

	return S_OK;
}

void CBaseUI::UpdateTransform()
{
	if (m_bUpdateTransform)
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
		m_bUpdateTransform = false;
	}
}

void CBaseUI::RenderUI()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix, OldProjection;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pBack->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pFront->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}
