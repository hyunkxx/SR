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
#include"EnermyMgr.h"
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
	m_pTransformCom->Set_Scale(VTXITV*(VTXCNTX - 1) *0.25, 0.f, VTXITV*(VTXCNTZ - 1) *0.25);
	m_pTransformCom->Set_Pos(VTXITV*(VTXCNTX - 1) *0.75, 0, VTXITV*(VTXCNTZ - 1) *0.75);

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

		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
		vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
		if (Temps.size() != 0)
		{
			CheckObject(OBJID::OBJID_BDENERMY);
			Occupation(OBJID::OBJID_BDENERMY);

		}
		if (Sours.size() != 0)
		{
			CheckObject(OBJID::OBJID_DEFAULT_ENERMY);
			Occupation(OBJID::OBJID_DEFAULT_ENERMY);
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
	switch (_ObjID)
	{

	case OBJID::OBJID_DEFAULT_ENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (dynamic_cast<CDefault_Enermy*>(*iter)->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && dynamic_cast<CDefault_Enermy*>(*iter)->Get_RightTopLocation() == true)
			{
				dynamic_cast<CDefault_Enermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
				dynamic_cast<CDefault_Enermy*>(*iter)->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
				m_iEnermyCount += 1;
			}
			if (dynamic_cast<CDefault_Enermy*>(*iter)->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && dynamic_cast<CDefault_Enermy*>(*iter)->Get_RightTopLocation() == false)
			{
				dynamic_cast<CDefault_Enermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
				m_iEnermyCount -= 1;
			}
		}
		break;
	}
	case OBJID::OBJID_BDENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightTopLocation() == true)
			{
				dynamic_cast<CBottomDirEnermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
				dynamic_cast<CBottomDirEnermy*>(*iter)->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP);
				m_iEnermyCount += 1;
			}
			if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_RIGHTTOP && dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightTopLocation() == false)
			{
				dynamic_cast<CBottomDirEnermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
				m_iEnermyCount -= 1;
			}

		}
		break;
	}
	default:
		break;
	}
}
void CRightTopLocation::Occupation(_int _ObjID)
{
	switch (_ObjID)
	{
	case OBJID::OBJID_DEFAULT_ENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (m_EnermyOccupation >= 100.f)
			{
				if (dynamic_cast<CDefault_Enermy*>(*iter)->Get_RightTopLocation() == true)
				{
					dynamic_cast<CDefault_Enermy*>(*iter)->Set_Action(AIACTION::AIACTION_WAIT);
				}
			}
		}
		break;
	}
	case OBJID::OBJID_BDENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (m_EnermyOccupation >= 100.f)
			{
				if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightTopLocation() == true)
				{
					dynamic_cast<CBottomDirEnermy*>(*iter)->Set_Action(AIACTION::AIACTION_WAIT);
				}
			}
		}
		break;
	}
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