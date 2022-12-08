#include "stdafx.h"
#include "..\Header\UI_Volume.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"

CUI_Volume::CUI_Volume(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Volume::CUI_Volume(const CUI_Volume& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Volume::~CUI_Volume()
{
}

HRESULT CUI_Volume::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	m_fScaleX = 250.f;
	m_fScaleY = 150.f;
	m_fScaleZ = 1.f;

	m_fPosX = 300.f;
	m_fPosY = 300.f;
	m_fPosZ = 0.f;
		
	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);



	return S_OK;
}

_int CUI_Volume::Update_Object(const _float & fTimeDelta)
{
	if (Get_DIKeyState_Custom(DIK_F3) == KEY_STATE::TAP)
	{
		m_bShow = !m_bShow;
	}
	if (m_bShow)
	{
		Move_Pos();
		Sound_Size_Control();
	}
	

	__super::Update_Object(fTimeDelta);
	return 0;
}

void CUI_Volume::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CUI_Volume::Render_Object(void)
{
	if (m_bShow)
	{
		
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		_matrix			ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();


		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);



	}
}

CUI_Volume * CUI_Volume::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Volume*		pInstance = new CUI_Volume(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("World_Bubble Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Volume::Move_Pos(void)
{

	//RECT		rcRect;
	//SetRect(&rcRect
	//	, _int(m_fPosX - m_fScaleX * 0.5f)
	//	, _int(m_fPosY - m_fScaleY * 0.5f)
	//	, _int(m_fPosX + m_fScaleX * 0.5f)
	//	, _int(m_fPosY - m_fScaleY * 0.5f + 15.f)
	//);

	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);
	//if (PtInRect(&rcRect, ptMouse))
	//{
	//	if(Get_DIMouseState(DIM_LB) == KEY_STATE::HOLD)
	//	{ 
	//		_float fX = _float(ptMouse.x - m_fPosX);
	//		_float fY = _float(ptMouse.y - m_fPosY);

	//		m_fPosX = _float(ptMouse.x + fX);
	//		m_fPosY = _float(ptMouse.y + fY);
	//		}		
	//}

}

void CUI_Volume::Sound_Size_Control(void)
{
//	_float					m_RatioX, m_RatioY, m_RatioZ;
//	_float					m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ;
	// 스태틱으로 한꺼번에 관리 해야 할것 같은데.
	// 전체 음량 조절 기능 (전부 같은 수로 통일)
	// 개별 사운드 조절, 배경음, 총알 소리, 환경음
	//RECT		rcRect;
	//SetRect(&rcRect
	//	,_int(m_fPosX - m_fScaleX * 0.5f )
	//	,_int(m_fPosY - m_fScaleY * 0.5f )
	//	,_int(m_fPosX + m_fScaleX * 0.5f)
	//	,_int(m_fPosY + m_fScaleY * 0.5f)
	//);
	//
	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);
	//
	//if ( PtInRect(&rcRect, ptMouse))
	//{
	//	
	//}






	
}





HRESULT CUI_Volume::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Volume_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Volume_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	return S_OK;
}

void CUI_Volume::Free(void)
{
	CGameObject::Free();
}
