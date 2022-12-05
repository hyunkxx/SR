#include "stdafx.h"
#include "..\Header\RightTopLocation.h"

#include "Export_Function.h"
#include"Default_Enermy.h"
#include"Management.h"
#include"Layer.h"
#include"Default_Enermy.h"
#include"BottomDirEnermy.h"
#include"Default_Ally.h"
#include"TestPlayer.h"
CRightTopLocation::CRightTopLocation(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CRightTopLocation::CRightTopLocation(const CRightTopLocation& rhs)
	: Engine::CGameObject(rhs)
{

}

CRightTopLocation::~CRightTopLocation()
{
}

HRESULT CRightTopLocation::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale((VTXCNTX - 1) *0.25, 0.f, (VTXCNTZ - 1) *0.25);
	m_pTransformCom->Set_Pos((VTXCNTX - 1) *0.75, 0, (VTXCNTZ - 1) *0.75);

	m_LocationState = LOCATIONSTATE::STATE_ENERMYHQ;
	return S_OK;
}

_int CRightTopLocation::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	if (m_Test == false)
	{
		m_Test = true;
	}
	else
	{
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"GameLogic");

		CGameObject* DETemp = Sour->Get_GameObject(L"Default_Enermy");
		CGameObject* BDETemp = Sour->Get_GameObject(L"BottomDirEnermy");
		CGameObject* DAllyTemp = Sour->Get_GameObject(L"Default_Ally");
		if (DETemp != nullptr)
		{
			CheckObject(OBJID::OBJID_DEFAULT_ENERMY);
			Occupation(OBJID::OBJID_DEFAULT_ENERMY);
		}
		if (BDETemp != nullptr)
		{
			CheckObject(OBJID::OBJID_BDENERMY);
			Occupation(OBJID::OBJID_BDENERMY);
		}
		if (DAllyTemp != nullptr)
		{
			CheckObject(OBJID::OBJID_DEFAULT_ALLY);
			Occupation(OBJID::OBJID_DEFAULT_ALLY);
		}
	}
	return OBJ_NOEVENT;
}

void CRightTopLocation::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_PRIORITY, this);
}

void CRightTopLocation::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pLocationCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}




void CRightTopLocation::CheckObject(_int _ObjID)
{
	CManagement* Temp = CManagement::GetInstance();
	CLayer*      Sour = Temp->Find_Layer(L"GameLogic");
	CGameObject* Dest;
	switch (_ObjID)
	{
	case OBJID::OBJID_PLAYER:
	{
		CTestPlayer* TestPlayerTemp = dynamic_cast<CTestPlayer*>(Dest = Sour->Get_GameObject(L"TestPlayer"));
		break;
	}
	case OBJID::OBJID_DEFAULT_ALLY:
	{
		CDefault_Ally* AllyTemp = dynamic_cast<CDefault_Ally*>(Dest = Sour->Get_GameObject(L"Default_Ally"));
		if (AllyTemp->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP&& AllyTemp->Get_RightTopLocation() == true)
		{
			AllyTemp->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
			AllyTemp->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
			m_iAllyCount += 1;
		}
		if (AllyTemp->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && AllyTemp->Get_RightTopLocation() == false)
		{
			AllyTemp->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
			m_iAllyCount -= 1;
		}
		break;
	}
	case OBJID::OBJID_DEFAULT_ENERMY:
	{CDefault_Enermy* DETemp = dynamic_cast<CDefault_Enermy*>(Dest = Sour->Get_GameObject(L"Default_Enermy"));
	if (DETemp->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && DETemp->Get_RightTopLocation() == true)
	{
		DETemp->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
		DETemp->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
		m_iEnermyCount += 1;
	}
	if (DETemp->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && DETemp->Get_RightTopLocation() == false)
	{
		DETemp->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
		m_iEnermyCount -= 1;
	}
	break;
	}
	case OBJID::OBJID_BDENERMY:
	{CBottomDirEnermy* BDETemp = dynamic_cast<CBottomDirEnermy*>(Dest = Sour->Get_GameObject(L"BottomDirEnermy"));
	if (BDETemp->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && BDETemp->Get_RightTopLocation() == true)
	{
		BDETemp->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
		BDETemp->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
		m_iEnermyCount += 1;
	}
	if (BDETemp->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && BDETemp->Get_RightTopLocation() == false)
	{
		BDETemp->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
		m_iEnermyCount -= 1;
	}
	break;
	}

	default:
		break;
	}
}
void CRightTopLocation::Occupation(_int _ObjID)
{
	CManagement* Temp = CManagement::GetInstance();
	CLayer*      Sour = Temp->Find_Layer(L"GameLogic");
	CGameObject* Dest;

	switch (_ObjID)
	{
	case OBJID::OBJID_PLAYER:
	{
		CTestPlayer* TestPlayerTemp = dynamic_cast<CTestPlayer*>(Dest = Sour->Get_GameObject(L"TestPlayer"));
		break;
	}
	case OBJID::OBJID_DEFAULT_ALLY:
	{
		CDefault_Ally* AllyTemp = dynamic_cast<CDefault_Ally*>(Dest = Sour->Get_GameObject(L"Default_Ally"));
		if (m_AllyOccupation >= 100.f)
			if (AllyTemp->Get_RightTopLocation() == true)
				AllyTemp->Set_Action(AIACTION::AIACTION_WAIT);
		break;
	}
	case OBJID::OBJID_DEFAULT_ENERMY:
	{CDefault_Enermy* DETemp = dynamic_cast<CDefault_Enermy*>(Dest = Sour->Get_GameObject(L"Default_Enermy"));
	if (m_iEnermyCount >= 1 && m_iAllyCount <= 0)
		if (DETemp->Get_RightTopLocation() == true)
			DETemp->Set_Action(AIACTION::AIACTION_OCCOPATION);

	if (m_iAllyCount >= 1 && m_iEnermyCount <= 0)
		if (DETemp->Get_RightTopLocation() == true)
			DETemp->Set_Action(AIACTION::AIACTION_DEFENSE);

	if (m_EnermyOccupation >= 100.f)
		if (DETemp->Get_RightTopLocation() == true)
			DETemp->Set_Action(AIACTION::AIACTION_WAIT);
	break;
	}
	case OBJID::OBJID_BDENERMY:
	{CBottomDirEnermy* BDETemp = dynamic_cast<CBottomDirEnermy*>(Dest = Sour->Get_GameObject(L"BottomDirEnermy"));
	if (m_iEnermyCount >= 1 && m_iAllyCount <= 0)
		if (BDETemp->Get_RightTopLocation() == true)
			BDETemp->Set_Action(AIACTION::AIACTION_OCCOPATION);

	if (m_iAllyCount >= 1 && m_iEnermyCount <= 0)
		if (BDETemp->Get_RightTopLocation() == true)
			BDETemp->Set_Action(AIACTION::AIACTION_DEFENSE);

	if (m_EnermyOccupation >= 100.f)
		if (BDETemp->Get_RightTopLocation() == true)
			BDETemp->Set_Action(AIACTION::AIACTION_WAIT);
	break;
	}
	if (m_iEnermyCount >= 1 && m_iAllyCount <= 0)
		m_EnermyOccupation += 0.5f*m_iEnermyCount;
	if (m_iAllyCount >= 1 && m_iEnermyCount <= 0)
		m_AllyOccupation += 0.5f*m_iAllyCount;
	if (m_EnermyOccupation >= 100.f)
		m_EnermyOccupation = 100.f;
	m_LocationState = LOCATIONSTATE::STATE_ENERMYHQ;
	if (m_AllyOccupation >= 100.f)
		m_AllyOccupation = 100.f;
	m_LocationState = LOCATIONSTATE::STATE_ALLY;
	}
}

CRightTopLocation* CRightTopLocation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRightTopLocation*		pInstance = new CRightTopLocation(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("RightLocation Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CRightTopLocation::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pLocationCom = dynamic_cast<CLocation*>(Clone_Prototype(L"Proto_Location"));
	NULL_CHECK_RETURN(m_pLocationCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Location", pComponent });

	return S_OK;
}
void CRightTopLocation::Free(void)
{
	__super::Free();
}