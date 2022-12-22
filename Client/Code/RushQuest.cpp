#include "stdafx.h"
#include "..\Header\RushQuest.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"
#include "RushMode.h"

CRushQuest::CRushQuest(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CRushQuest::CRushQuest(const CRushQuest& rhs)
	: Engine::CGameObject(rhs)
{
}


CRushQuest::~CRushQuest()
{
}

CRushQuest* CRushQuest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRushQuest*	pInstance = new CRushQuest(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CRushQuest Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CRushQuest::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 110.f;
	m_fScaleY = 40.f;
	m_fScaleZ = 0.01f;

	m_fPosX = 100.f;
	m_fPosY = 40.f;
	m_fPosZ = 0.5f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}

_int CRushQuest::Update_Object(const _float & fTimeDelta)
{
	if (CRushMode::GetInstance()->m_nDeadCount >= 15)
		return 0;

	__super::Update_Object(fTimeDelta);

	m_pPlayer = Engine::Get_Object(L"GameLogic", L"PlayerVehicle");


	return 0;
}

void CRushQuest::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CRushQuest::Render_Object(void)
{
	if(CRushMode::GetInstance()->m_nDeadCount >= 8)
		return;
	
	wstring strQuestTitle = L"미션 목표";
	wstring strQuest1 = L"적을 모두 섬멸하세요";
	wstring strQuest2 = L"처치한 적 " + to_wstring(CRushMode::GetInstance()->m_nDeadCount) + L" / " + to_wstring(8);
	_vec2 vPos = { 10.f , 0.f };
	Engine::Render_Font(L"Font_Retro_slim", strQuestTitle.c_str(), &vPos, D3DCOLOR_RGBA(255, 255, 0, 255));
	vPos = { 10.f,  25.f };
	Engine::Render_Font(L"Font_Retro_slim", strQuest1.c_str(), &vPos, D3DCOLOR_RGBA(255, 0, 0, 255));
	vPos = { 10.f,  50.f };
	Engine::Render_Font(L"Font_Retro_slim", strQuest2.c_str(), &vPos, D3DCOLOR_RGBA(255, 255, 255, 255));

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

void CRushQuest::Free(void)
{
	__super::Free();
}

HRESULT CRushQuest::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"rect_texture", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"game_result"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"texture", pComponent });


	return S_OK;
}