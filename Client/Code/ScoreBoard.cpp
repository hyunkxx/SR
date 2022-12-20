#include "stdafx.h"
#include "..\Header\ScoreBoard.h"
#include "Export_Function.h"
#include "UI_Start.h"

CScoreBoard::CScoreBoard(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CScoreBoard::CScoreBoard(const CScoreBoard& rhs)
	: Engine::CGameObject(rhs)
{

}

CScoreBoard::~CScoreBoard()
{
}

HRESULT CScoreBoard::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fSizeX = 160.f;
	m_fSizeY = 50.0f;
	m_fX = 400.f;
	m_fY = 25.f;

	m_pTransform->Set_Scale(m_fSizeX*0.5f, m_fSizeY*0.5f, 1.f);
	m_pTransform->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.5f);


	return S_OK;
}



_int CScoreBoard::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);




	return OBJ_NOEVENT;
}

void CScoreBoard::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
	Add_RenderGroup(RENDER_UI, this);

}

void CScoreBoard::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
	_matrix	ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pTexture->Set_Texture();
	m_pRcTex->Render_Buffer();


	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//
	//_matrix	ViewMatrix, OldProjection, OldView;
	//D3DXMatrixIdentity(&ViewMatrix);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldView);

	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//m_pTexture->Set_Texture();
	//m_pRcTex->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);


	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CScoreBoard::Free(void)
{
	__super::Free();

}

CScoreBoard* CScoreBoard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScoreBoard*		pInstance = new CScoreBoard(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CScoreBoard Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


HRESULT CScoreBoard::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	//buffer
	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//Transform
	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_ScoreBoard"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ScoreBoard", pComponent });
	return S_OK;
}
