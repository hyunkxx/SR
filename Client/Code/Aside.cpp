#include "stdafx.h"
#include "..\Header\Aside.h"
#include "Export_Function.h"
#include "UI_Start.h"
#include"TempOccupationScore.h"
#include"Management.h"
#include"Layer.h"
#include"LeftTopLocation.h"
#include"RightLocation.h"
CAside::CAside(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CAside::CAside(const CAside& rhs)
	: Engine::CGameObject(rhs)
{

}

CAside::~CAside()
{
}

HRESULT CAside::Ready_Object(void)
{
	m_iColor = 0;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_iColor = 0;
	m_fSizeX = 2.f;
	m_fSizeY = 2.f;
	m_OrignfX = 370.f;
	m_OrignfY = 80.f;
	m_fX = m_OrignfX;
	m_fY = m_OrignfY;
	m_pTransformLogo->Set_Scale(20.f, 20.f, 1.f);

	m_pTransformCom->Rotation(ROTATION::ROT_Z, D3DXToRadian(45.f));
	m_pTransformRight->Rotation(ROTATION::ROT_Z, -D3DXToRadian(45.f));
	m_pTransformBottom->Rotation(ROTATION::ROT_Z, -D3DXToRadian(135.f));
	m_pTransformLeft->Rotation(ROTATION::ROT_Z, D3DXToRadian(135.f));

	m_pTransformCom->Set_Pos(-1.f, -1.f, -1.f);
	m_pTransformRight->Set_Pos(-1.f, -1.f, -1.f);
	m_pTransformBottom->Set_Pos(-1.f, -1.f, -1.f);
	m_pTransformLeft->Set_Pos(-1.f, -1.f, -1.f);

	m_pTransformLogo->Set_Pos(m_OrignfX - WINCX * 0.5f, -(m_OrignfY + 19.f) + WINCY * 0.5f, 0.5f);
	return S_OK;
}

_int CAside::Update_Object(const _float& fTimeDelta)
{

	CManagement* Temp = CManagement::GetInstance();
	CLayer*      Sour = Temp->Find_Layer(L"Environment");
	CGameObject* Dest;
	CLeftTopLocation* LLTTemp = dynamic_cast<CLeftTopLocation*>(Dest = Sour->Get_GameObject(L"LeftTopLocation"));
	_float AllyOcc = LLTTemp->Get_AllyOccupation();
	_float EnermyOcc = LLTTemp->Get_EnermyOccupation();

	__super::Update_Object(fTimeDelta);
	if (EnermyOcc > 0)
	{
		StartTime = EnermyOcc;
		m_iColor = 1;
	}
	else if (AllyOcc > 0)
	{
		StartTime = AllyOcc;
		m_iColor = 2;
	}


	if (Save>StartTime)
	{
		switch (m_iCount)
		{
		case 1:
		{
			if (StartTime <= 0.f)
			{
				m_bEnemyOcc = false;
			}
		}
		break;
		case 2:
		{
			if (StartTime <= 24.f &&RightEnd)
			{
				RightEnd = false;
				m_fX = m_OrignfX;
				m_fY = m_OrignfY;
			}
			if (StartTime <= 0.f && !RightEnd)
			{
				m_bEnemyOcc = false;
			}
		}
		break;
		case 3:
		{
			if (StartTime <= 48.f && BottomEnd)
			{
				BottomEnd = false;
				RightEnd = true;
				m_fX = m_OrignfX + 19.f;
				m_fY = m_OrignfY + 19.f;
			}
			if (StartTime <= 24.f&& RightEnd)
			{
				RightEnd = false;
				m_fX = m_OrignfX;
				m_fY = m_OrignfY;
			}

			if (StartTime <= 0 && !RightEnd && !BottomEnd)
			{
				m_bEnemyOcc = false;
			}
		}
		break;
		case 4:
		{

			if (StartTime <= 72.f && LeftEnd)
			{
				LeftEnd = false;
				BottomEnd = true;
				m_fX = m_OrignfX;
				m_fY = m_OrignfY + 38.f;
			}
			if (StartTime <= 48.f && BottomEnd)
			{
				BottomEnd = false;
				RightEnd = true;
				m_fX = m_OrignfX + 19.f;
				m_fY = m_OrignfY + 19.f;
			}
			if (StartTime <= 24.f && RightEnd)
			{
				RightEnd = false;
				m_fX = m_OrignfX;
				m_fY = m_OrignfY;
			}

			if (StartTime <= 0 && !RightEnd && !BottomEnd && !LeftEnd)
			{

				m_bEnemyOcc = false;
			}
		}
		break;
		}
	}

	if (StartTime >= 24.f && !RightEnd && !BottomEnd && !LeftEnd)
	{

		RightEnd = true;
		BottomEnd = false;
		LeftEnd = false;
		m_fX = m_OrignfX + 19.f;
		m_fY = m_OrignfY + 19.f;
		Check = false;
	}
	if (StartTime >= 48.f && RightEnd)
	{
		RightEnd = false;
		BottomEnd = true;
		LeftEnd = false;
		m_fX = m_OrignfX;
		m_fY = m_OrignfY + 38.f;
		Check = false;
	}
	if (StartTime >= 72.f && BottomEnd)
	{
		RightEnd = false;
		BottomEnd = false;
		LeftEnd = true;
		m_fX = m_OrignfX - 19.f;
		m_fY = m_OrignfY + 19.f;
		Check = false;
	}
	if (StartTime >= 96.f && LeftEnd)
	{
		StartTime = 100.f;
	}
	if (StartTime < 24.f)
	{
		_float Dist = StartTime / 24.f;
		SaveDist = Dist;
		_float ResultY = Lerp(0.f, 10.f, Dist);

		_float Temp = sqrtf(2) *fabs(ResultY);

		m_pTransformCom->Set_Scale(1.f, Temp, 1.f);
	
		m_pTransformCom->Set_Pos((m_fX + ResultY) - WINCX * 0.5f, -(m_fY + ResultY) + WINCY * 0.5f, 0.5f);
		if (!Check)
		{
			m_iCount += 1;
			Check = true;
		}
	}
	else if (StartTime < 48.f)
	{
		_float Dist = (StartTime - 24.f) / 24.f;
		SaveDist = Dist;
		_float ResultY = Lerp(0.f, 10.f, Dist);


		_float Temp = sqrtf(2) *fabs(ResultY);

		m_pTransformRight->Set_Scale(1.f, Temp, 1.f);
		m_pTransformRight->Set_Pos((m_fX - ResultY) - WINCX * 0.5f, -(m_fY + ResultY) + WINCY * 0.5f, 0.5f);
		if (!Check)
		{
			m_iCount += 1;
			Check = true;
		}
	}
	else if (StartTime < 72.f)
	{
		_float Dist = (StartTime - 48.f) / 24.f;
		SaveDist = Dist;
		_float ResultY = Lerp(0.f, 10.f, Dist);


		_float Temp = sqrtf(2) *fabs(ResultY);

		m_pTransformBottom->Set_Scale(1.f, Temp, 1.f);
	
			m_pTransformBottom->Set_Pos((m_fX - ResultY) - WINCX * 0.5f, -(m_fY - ResultY) + WINCY * 0.5f, 0.5f);
		if (!Check)
		{
			m_iCount += 1;
			Check = true;
		}
	}
	else if (StartTime < 96.f)
	{
		_float Dist = (StartTime - 72.f) / 24.f;
		SaveDist = Dist;
		_float ResultY = Lerp(0.f, 10.f, Dist);

		_float Temp = sqrtf(2) *fabs(ResultY);

		m_pTransformLeft->Set_Scale(1.f, Temp, 1.f);

	
		m_pTransformLeft->Set_Pos((m_fX + ResultY) - WINCX * 0.5f, -(m_fY - ResultY) + WINCY * 0.5f, 0.5f);
		if (!Check)
		{
			m_iCount += 1;
			Check = true;
		}
	}

	Add_RenderGroup(RENDER_UI, this);
	return OBJ_NOEVENT;
}

void CAside::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
	Save = StartTime;
}

void CAside::Render_Object(void)
{
	_matrix	ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformLogo->Get_WorldMatrix());

	m_pLogoTexture->Set_Texture();
	m_pRcTexLogo->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTexture->Set_Texture(m_iColor);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformRight->Get_WorldMatrix());

	m_pTexture->Set_Texture(m_iColor);
	m_pRcTexRight->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBottom->Get_WorldMatrix());

	m_pTexture->Set_Texture(m_iColor);
	m_pRcTexBottom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformLeft->Get_WorldMatrix());

	m_pTexture->Set_Texture(m_iColor);
	m_pRcTexLeft->Render_Buffer();

}

CAside* CAside::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAside*		pInstance = new CAside(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CAside Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


HRESULT CAside::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	//buffer
	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pRcTexRight = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTexRight, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RRcTex", pComponent });

	pComponent = m_pRcTexBottom = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTexBottom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_BRcTex", pComponent });

	pComponent = m_pRcTexLeft = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTexLeft, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LRcTex", pComponent });

	pComponent = m_pRcTexLogo = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTexLeft, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LogoRcTex", pComponent });

	//Transform
	pComponent = m_pTransformCom = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformRight = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformRight, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RTransform", pComponent });

	pComponent = m_pTransformBottom = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBottom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_BTransform", pComponent });

	pComponent = m_pTransformLeft = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformLeft, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_LTransform", pComponent });

	pComponent = m_pTransformLogo = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformLeft, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_LogoTransform", pComponent });


	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_OccupationColor"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_OccupationColor", pComponent });

	pComponent = m_pLogoTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Aside"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Aside", pComponent });

	return S_OK;
}

void CAside::Free(void)
{
	__super::Free();
}
