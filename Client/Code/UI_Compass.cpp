#include "stdafx.h"
#include "..\Header\UI_Compass.h"
#include "Stage.h"
#include "Export_Function.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"
#include "UI_Volume.h"
CUI_Compass::CUI_Compass(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Compass::CUI_Compass(const CUI_Compass& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Compass::~CUI_Compass()
{
}

HRESULT CUI_Compass::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 70.f;
	m_fScaleY = 70.f;
	m_fScaleZ = 1.f;

	m_fPosX = 730.f;
	m_fPosY = 530.f;
	m_fPosZ = 0.01f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_szlastTankName = m_szTankName = CUI_FontMgr::GetInstance()->Get_Tank_Name();

	if (m_szTankName == L"Humvee")
	{
		iRotaion = 40;
	}
	else if (m_szTankName == L"A - 44")
	{
		iRotaion = 40;
	}
	else if (m_szTankName == L"TE6E4")
	{
		iRotaion = 30;
	}
	else if (m_szTankName == L"KING TIGER")
	{
		iRotaion = 20;
	}
	else if (m_szTankName == L"M55")
	{
		iRotaion = 10;
	}


	return S_OK;
}


_int CUI_Compass::Update_Object(const _float & fTimeDelta)
{

	

	
	






	m_szTankName = CUI_FontMgr::GetInstance()->Get_Tank_Name();

	if (m_szlastTankName != m_szTankName)
	{
		m_pTransform->Rotation(ROT_Z, 0.f - m_pTransform->Get_Angle(ROT_Z));
		m_szlastTankName = m_szTankName;

	}


	if (m_szTankName == L"Humvee")
	{
		iRotaion = 40;
	}
	else if (m_szTankName == L"A - 44")
	{
		iRotaion = 40;
	}
	else if (m_szTankName == L"TE6E4")
	{
		iRotaion = 30;
	}
	else if (m_szTankName == L"KING TIGER")
	{
		iRotaion = 20;
	}
	else if (m_szTankName == L"M55")
	{
		iRotaion = 10;
	}
	_float fSound = 1.f;

	if (CUI_Volume::s_fAllSound == 0.f)
	{
		fSound = 0.f;
	}
	else if (CUI_Volume::s_fAllSound != 0.f)
	{
		fSound = 1.f;
	}



	if (Get_DIKeyState_Custom(DIK_A) == KEY_STATE::HOLD)
	{
		m_pTransform->Rotation(ROT_Z, D3DXToRadian((iRotaion)*fTimeDelta));
		//m_pTransform->Rotation(ROT_Z, vAngle_F);
		//PlaySound_SR(L"Rotaion.mp3", ROTAION_SOUND, fSound);

	}
	if (Get_DIKeyState_Custom(DIK_D) == KEY_STATE::HOLD)
	{
		m_pTransform->Rotation(ROT_Z, D3DXToRadian( -(iRotaion) * fTimeDelta));
		//PlaySound_SR(L"Rotaion.mp3", ROTAION_SOUND, fSound);
	}
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_Compass::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	
	Add_RenderGroup(RENDER_UI, this);
	
}

void CUI_Compass::Render_Object(void)
{
	if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{

		//Render_Font()

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();


	}

}

CUI_Compass * CUI_Compass::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Compass*		pInstance = new CUI_Compass(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Compass Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_Compass::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_Compass_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_Compass_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Compass", pComponent });


	return S_OK;
}

void CUI_Compass::Free(void)
{
	CGameObject::Free();
}
