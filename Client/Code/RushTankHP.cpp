#include "stdafx.h"
#include "..\Header\RushTankHP.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"
#include "RushMode.h"

CRushTankHP::CRushTankHP(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CRushTankHP::CRushTankHP(const CRushTankHP& rhs)
	: Engine::CGameObject(rhs)
{
}


CRushTankHP::~CRushTankHP()
{
}

CRushTankHP* CRushTankHP::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRushTankHP*	pInstance = new CRushTankHP(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CRushTankHP Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CRushTankHP::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fPosX = WINCX * 0.5f;
	m_fPosY = WINCY - 10.f;
	m_fPosZ = 0.0f;

	m_fScaleX = 400.f;
	m_fScaleY = 10.f;
	m_fScaleZ = 0.01f;
	m_pBackTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pBackTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_fScaleX = 395.f;
	m_fScaleY = 5.f;
	m_fScaleZ = 0.1f;

	m_fLerpX = 395.f;
	m_fLerpPosX = m_fPosX;

	m_pBackBlackTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pBackBlackTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_pBackYellowTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pBackYellowTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_pFrontTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pFrontTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CRushTankHP::Update_Object(const _float & fTimeDelta)
{
	if (Get_DIKeyState_Custom(DIK_F8) == KEY_STATE::TAP)
	{
		CRushMode::GetInstance()->m_fPlayerCurHP += 500.f;
	}

	if (Get_DIKeyState_Custom(DIK_F7) == KEY_STATE::TAP)
	{
		CRushMode::GetInstance()->m_fPlayerCurHP -= 500.f;
	}

	curPer = (CRushMode::GetInstance()->m_fPlayerCurHP / CRushMode::GetInstance()->m_fPlayerMaxHP)  * 395.f;

	float fScaleX = Utility::Lerp(m_fLerpX, curPer, fTimeDelta);

	m_fScaleX = curPer;
	float fX = 395.f - m_fScaleX;
	m_fPosX = (WINCX * 0.5f) - fX;
	m_pFrontTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pFrontTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_fLerpX = fScaleX;
	_float fLerpX = 395.f - m_fLerpX;
	m_fLerpPosX = (WINCX * 0.5f) - fLerpX;
	m_pBackYellowTransform->Set_Scale(m_fLerpX, m_fScaleY, m_fScaleZ);
	m_pBackYellowTransform->Set_Pos(m_fLerpPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	//if (curPer <= 0.f)
	//{
	//	CGameMode::GetInstance()->m_eGameResult = CGameMode::GAME_RESULT::LOSE;
	//	CGameMode::GetInstance()->m_bGameEnd = true;
	//}

	__super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CRushTankHP::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CRushTankHP::Render_Object(void)
{
	_matrix	ViewMatrix, OldProjection, OldView;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBackTransform->Get_WorldMatrix());
	m_pBack->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBackBlackTransform->Get_WorldMatrix());
	m_pBackBlack->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBackYellowTransform->Get_WorldMatrix());
	m_pBackYellow->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pFrontTransform->Get_WorldMatrix());
	m_pFront->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}

void CRushTankHP::Free(void)
{
	__super::Free();
}

HRESULT CRushTankHP::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"rect_texture", pComponent });

	pComponent = m_pBackTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBackTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform_back", pComponent });

	pComponent = m_pBackBlackTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBackBlackTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform_back_black", pComponent });

	pComponent = m_pBackYellowTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBackYellowTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform_back_yelllow", pComponent });

	pComponent = m_pFrontTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pFrontTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform_front", pComponent });

	pComponent = m_pBack = static_cast<CTexture*>(Clone_Prototype(L"base_back"));
	NULL_CHECK_RETURN(m_pBack, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"back", pComponent });

	pComponent = m_pBackBlack = static_cast<CTexture*>(Clone_Prototype(L"base_black_back"));
	NULL_CHECK_RETURN(m_pBackBlack, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"back_black", pComponent });

	pComponent = m_pBackYellow = static_cast<CTexture*>(Clone_Prototype(L"base_yellow_back"));
	NULL_CHECK_RETURN(m_pBackYellow, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"back_yellow", pComponent });

	pComponent = m_pFront = static_cast<CTexture*>(Clone_Prototype(L"Proto_World_Hp_Tex"));
	NULL_CHECK_RETURN(m_pFront, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"front", pComponent });

	return S_OK;
}