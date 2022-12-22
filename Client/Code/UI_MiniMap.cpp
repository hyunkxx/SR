#include "stdafx.h"
#include "..\Header\UI_MiniMap.h"
#include "Stage.h"
#include "Tank_01.h"
#include "Export_Function.h"
#include "TankManager.h"
#include "UI_FontMgr.h"
#include "LeftLocation.h"
#include "LeftTopLocation.h"
#include "RightLocation.h"
#include "RightTopLocation.h"
#include "GameMode.h"
#include "UI_Volume.h"

CUI_MiniMap::CUI_MiniMap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_MiniMap::CUI_MiniMap(const CUI_MiniMap& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_MiniMap::~CUI_MiniMap()
{
}

HRESULT CUI_MiniMap::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	// minimap back
	m_fScaleX = 80.f;
	m_fScaleY = 57.5f;
	m_fScaleZ = 0.f;

	m_fPosX = 720.f;
	m_fPosY = 542.5f;
	m_fPosZ = 0.9f;

	// minimap Curcle common
	m_fCurcleScaleX = 12.5f;
	m_fCurcleScaleY = 12.5f;
	m_fCurcleScaleZ = 0.f;
	m_fCurclePosZ = 0.8f;

	// minimap curcle1
	m_fAPosX = 680.f;
	m_fAPosY = 514.f;

	// minimap curcle2
	m_fBPosX = 760.f;
	m_fBPosY = 571.f;

	// minimap back
	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	// minimap curcle1
	m_pATransform->Set_Scale(m_fCurcleScaleX, m_fCurcleScaleY, m_fCurcleScaleZ);
	m_pATransform->Set_Pos(m_fAPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fAPosY, m_fCurclePosZ);

	// minimap curcle2
	m_pBTransform->Set_Scale(m_fCurcleScaleX, m_fCurcleScaleY, m_fCurcleScaleZ);
	m_pBTransform->Set_Pos(m_fBPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fBPosY, m_fCurclePosZ);

	return S_OK;
}


_int CUI_MiniMap::Update_Object(const _float & fTimeDelta)
{
	CGameObject* pGameLogicLT = Engine::Get_Object(L"Environment", L"LeftTopLocation");
	CGameObject* pGameLogicRB = Engine::Get_Object(L"Environment", L"RightLocation");

	_float LeftTop_Ally = static_cast<CLeftTopLocation*>(pGameLogicLT)->Get_AllyOccupation();
	_float LeftTop_Ener = static_cast<CLeftTopLocation*>(pGameLogicLT)->Get_EnermyOccupation();

	if (LeftTop_Ally > LeftTop_Ener)
	{
		m_iASite = 2;
	}
	else if (LeftTop_Ally < LeftTop_Ener)
	{
		m_iASite = 1;
	}
	else if (LeftTop_Ally == LeftTop_Ener)
	{
		m_iASite = 0;
	}
	_float Right_Ally = static_cast<CRightLocation*>(pGameLogicRB)->Get_AllyOccupation();
	_float Right_Ener = static_cast<CRightLocation*>(pGameLogicRB)->Get_EnermyOccupation();

	if (Right_Ally > Right_Ener)
	{
		m_iBSite = 2;
	}
	else if (Right_Ally < Right_Ener)
	{
		m_iBSite = 1;
	}
	else if (LeftTop_Ally == LeftTop_Ener)
	{
		m_iBSite = 0;
	}

	//if (Get_DIKeyState_Custom(DIK_NUMPAD1) == KEY_STATE::TAP)
	//{
	//	m_iASite = 0;
	//}
	//if (Get_DIKeyState_Custom(DIK_NUMPAD2) == KEY_STATE::TAP)
	//{
	//	m_iASite = 1;
	//}
	//if (Get_DIKeyState_Custom(DIK_NUMPAD3) == KEY_STATE::TAP)
	//{
	//	m_iASite = 2;
	//}
	//if (Get_DIKeyState_Custom(DIK_NUMPAD4) == KEY_STATE::TAP)
	//{
	//	m_iBSite = 0;
	//}
	//if (Get_DIKeyState_Custom(DIK_NUMPAD5) == KEY_STATE::TAP)
	//{
	//	m_iBSite = 1;
	//}
	//if (Get_DIKeyState_Custom(DIK_NUMPAD6) == KEY_STATE::TAP)
	//{
	//	m_iBSite = 2;
	//}

	_float fSound = 0.02f;
	
	if (CUI_Volume::s_fAllSound == 0.f)
	{
		fSound = 0.f;
	}
	else if (CUI_Volume::s_fAllSound != 0.f)
	{
		fSound = 0.02f;
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= 1.f)
	{
		if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
		{
			CGameObject* pPlayer = Get_Object(L"GameLogic", L"PlayerVehicle");
			if (!(pPlayer->Get_Dead()))
				PlaySound_SR(L"Rader2.mp3", RADER_SOUND, fSound);
			m_fTime = 0.f;
		}
	}


	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_MiniMap::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_PRIORITY, this);

}

void CUI_MiniMap::Render_Object(void)
{
	if (CGameMode::GetInstance()->m_bGameEnd)
		return;

	if ((Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA) || (Engine::Get_Camera_ID() == CAMERA_ID::AH_64A_CAMERA) || (Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA))
	{
		_matrix OldViewMatrix, OldProjMatrix;
		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		// minimap back
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();

		// minimap curcle1			
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pATransform->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pATexture->Set_Texture(m_iASite);
		m_pRcTex->Render_Buffer();

		// minimap curcle2
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBTransform->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pBTexture->Set_Texture(m_iBSite);
		m_pRcTex->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	}

}

CUI_MiniMap * CUI_MiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_MiniMap*		pInstance = new CUI_MiniMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Minimap Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_MiniMap::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_MiniMap_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_MiniMap_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_MiniMap", pComponent });

	// minimap curcle1		
	pComponent = m_pATexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_MiniMap_A_Tex"));
	NULL_CHECK_RETURN(m_pATexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_MiniMap_A_Tex", pComponent });

	pComponent = m_pATransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pATransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_MiniMap_A", pComponent });

	// minimap curcle2
	pComponent = m_pBTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_MiniMap_B_Tex"));
	NULL_CHECK_RETURN(m_pBTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_MiniMap_B_Tex", pComponent });

	pComponent = m_pBTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_MiniMap_B", pComponent });


	return S_OK;
}

void CUI_MiniMap::Free(void)
{
	Safe_Release(m_pATexture);
	Safe_Release(m_pATransform);
	Safe_Release(m_pBTexture);
	Safe_Release(m_pBTransform);
	CGameObject::Free();
}
