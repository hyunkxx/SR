#include "stdafx.h"
#include "..\Header\TempOccupationScore.h"
#include "Export_Function.h"
#include"Management.h"
#include"Layer.h"
#include"LeftLocation.h"
#include"LeftTopLocation.h"
#include"RightLocation.h"
#include"RightTopLocation.h"
#include "UI_Start.h"
CTempOccupationScore::CTempOccupationScore(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTempOccupationScore::CTempOccupationScore(const CTempOccupationScore& rhs)
	: Engine::CGameObject(rhs)
{

}

CTempOccupationScore::~CTempOccupationScore()
{
}

HRESULT CTempOccupationScore::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fSizeX = 100.f;
	m_fSizeY = 10.0f;
	m_fX = 70.f;
	m_fY = 200.f;

	m_pTransformLeftCom->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransformLeftCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	m_fX = 70.f;
	m_fY = 220.f;
	m_pTransformRightCom->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransformRightCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	m_fX = 70.f;
	m_fY = 240.f;
	m_pTransformLeftTopCom->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransformLeftTopCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	m_fX = 70.f;
	m_fY = 260.f;
	m_pTransformRightTopCom->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransformRightTopCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);
	return S_OK;
}

HRESULT CTempOccupationScore::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fSizeX = 100.f;
	m_fSizeY = 10.0f;
	m_fX = 70.f;
	m_fY = 200.f;


	return S_OK;
}


_int CTempOccupationScore::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	CManagement* Temp = CManagement::GetInstance();
	CLayer*      Sour = Temp->Find_Layer(L"Environment");
	CGameObject* Dest;
	CLeftTopLocation* LLTTemp = dynamic_cast<CLeftTopLocation*>(Dest = Sour->Get_GameObject(L"LeftTopLocation"));
	_float AllyOccupation = LLTTemp->Get_AllyOccupation();
	_float EnermyOccupation = LLTTemp->Get_EnermyOccupation();

	if (AllyOccupation > EnermyOccupation)
	{
		m_fSizeX = AllyOccupation;
		m_sLeftTop = 2;
	}
	else if (AllyOccupation < EnermyOccupation)
	{
		m_fSizeX = EnermyOccupation;
		m_sLeftTop = 1;
	}
	m_fX = m_fSizeX*0.5f + 20;
	m_fY = 240.f;;
	m_pTransformLeftTopCom->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransformLeftTopCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	CLeftLocation* LLTemp = dynamic_cast<CLeftLocation*>(Dest = Sour->Get_GameObject(L"LeftLocation"));
	AllyOccupation = LLTemp->Get_AllyOccupation();
	EnermyOccupation = LLTemp->Get_EnermyOccupation();
	if (AllyOccupation > EnermyOccupation)
	{
		m_fSizeX = AllyOccupation;
		m_sLeft = 2;
	}
	else if (AllyOccupation < EnermyOccupation)
	{
		m_fSizeX = EnermyOccupation;
		m_sLeft = 1;
	}
	m_fX = m_fSizeX*0.5f + 20;
	m_fY = 200.f;;
	m_pTransformLeftCom->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransformLeftCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	CRightLocation* LRTemp = dynamic_cast<CRightLocation*>(Dest = Sour->Get_GameObject(L"RightLocation"));
	AllyOccupation = LRTemp->Get_AllyOccupation();
	EnermyOccupation = LRTemp->Get_EnermyOccupation();
	if (AllyOccupation > EnermyOccupation)
	{
		m_fSizeX = AllyOccupation;
		m_sRight = 2;
	}
	else if (AllyOccupation < EnermyOccupation)
	{
		m_fSizeX = EnermyOccupation;
		m_sRight = 1;
	}
	m_fX = m_fSizeX*0.5f + 20;
	m_fY = 220.f;;
	m_pTransformRightCom->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransformRightCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	CRightTopLocation* LRTTemp = dynamic_cast<CRightTopLocation*>(Dest = Sour->Get_GameObject(L"RightTopLocation"));
	AllyOccupation = LRTTemp->Get_AllyOccupation();
	EnermyOccupation = LRTTemp->Get_EnermyOccupation();
	if (AllyOccupation > EnermyOccupation)
	{
		m_fSizeX = AllyOccupation;
		m_sRightTop = 2;
	}
	else if (AllyOccupation < EnermyOccupation)
	{
		m_fSizeX = EnermyOccupation;
		m_sRightTop = 1;
	}
	m_fX = m_fSizeX*0.5f + 20;
	m_fY = 260.f;;
	m_pTransformRightTopCom->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransformRightTopCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);




	return OBJ_NOEVENT;
}

void CTempOccupationScore::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CTempOccupationScore::Render_Object(void)
{
	CGameObject* pHelpWin = Engine::Get_Object(L"UI", L"Start_UI");
	_bool showF1Win = static_cast<CUI_Start*>(pHelpWin)->Get_HelpWin();

	if (!showF1Win)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformLeftCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture(m_sLeft);
		m_pBufferLeftCom->Render_Buffer();
	
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformRightCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture(m_sRight);
		m_pBufferRightCom->Render_Buffer();
	
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformRightTopCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture(m_sRightTop);
		m_pBufferRighttopCom->Render_Buffer();
	
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformLeftTopCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture(m_sLeftTop);
		m_pBufferLefttopCom->Render_Buffer();
		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);
	
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


	}


}

void CTempOccupationScore::Free(void)
{
	__super::Free();

}

CTempOccupationScore* CTempOccupationScore::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTempOccupationScore*		pInstance = new CTempOccupationScore(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUi Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTempOccupationScore * CTempOccupationScore::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CTempOccupationScore*		pInstance = new CTempOccupationScore(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		MSG_BOX("CUi Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CTempOccupationScore::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	//buffer
	pComponent = m_pBufferLeftCom = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferLeftCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pBufferRightCom = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferRightCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"RProto_RcTex", pComponent });

	pComponent = m_pBufferLefttopCom = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferLeftCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LTProto_RcTex", pComponent });

	pComponent = m_pBufferRighttopCom = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferRighttopCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"RTProto_RcTex", pComponent });



	//Transform
	pComponent = m_pTransformLeftCom = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformLeftCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformRightCom = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformRightCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RTransform", pComponent });

	pComponent = m_pTransformLeftTopCom = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformLeftTopCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_LTTransform", pComponent });

	pComponent = m_pTransformRightTopCom = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformRightTopCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RTTransform", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(Clone_Prototype(L"Proto_OccupationColor"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_OccupationColor", pComponent });
	return S_OK;
}
