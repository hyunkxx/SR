#include "stdafx.h"
#include "..\Header\LeftLocation.h"
#include "Export_Function.h"
#include"Default_Enermy.h"
#include"Management.h"
#include"Layer.h"
#include"Default_Enermy.h"
#include"BottomDirEnermy.h"
#include"Default_Ally.h"
#include"TestPlayer.h"
CLeftLocation::CLeftLocation(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CLeftLocation::CLeftLocation(const CLeftLocation& rhs)
	: Engine::CGameObject(rhs)
{

}

CLeftLocation::~CLeftLocation()
{
}

HRESULT CLeftLocation::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(VTXITV*(VTXCNTX - 1) *0.25, 0.f, VTXITV*(VTXCNTZ - 1) *0.25);
	m_pTransformCom->Set_Pos((VTXITV*(VTXCNTX - 1)*0.25), 0, (VTXITV*(VTXCNTZ - 1)) *0.25);

	m_LocationState = LOCATIONSTATE::STATE_ALLYHQ;
	return S_OK;
}

_int CLeftLocation::Update_Object(const _float& fTimeDelta)
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
		vector<CGameObject*>Dests = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
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
		if (Dests.size() != 0)
		{

			CheckObject(OBJID::OBJID_DEFAULT_ALLY);
			Occupation(OBJID::OBJID_DEFAULT_ALLY);
		}



	}
	return OBJ_NOEVENT;



}



void CLeftLocation::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_PRIORITY, this);
}

void CLeftLocation::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	m_pLocationCom->Render_Buffer();



	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}



void CLeftLocation::CheckObject(_int _ObjID)
{

	switch (_ObjID)
	{
	case OBJID::OBJID_DEFAULT_ALLY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (dynamic_cast<CDefault_Ally*>(*iter)->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_LEFT && dynamic_cast<CDefault_Ally*>(*iter)->Get_LeftLocation() == true)
			{
				dynamic_cast<CDefault_Ally*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_LEFT);
				dynamic_cast<CDefault_Ally*>(*iter)->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_LEFT);

				m_iAllyCount += 1;
			}
			if (dynamic_cast<CDefault_Ally*>(*iter)->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_LEFT && dynamic_cast<CDefault_Ally*>(*iter)->Get_LeftLocation() == false)
			{
				dynamic_cast<CDefault_Ally*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
				m_iAllyCount -= 1;
			}

		}
		break;
	}
	case OBJID::OBJID_DEFAULT_ENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (static_cast<CDefault_Enermy*>(*iter)->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_LEFT && static_cast<CDefault_Enermy*>(*iter)->Get_LeftLocation() == true)
			{
				static_cast<CDefault_Enermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_LEFT);
				static_cast<CDefault_Enermy*>(*iter)->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_LEFT);
				m_iEnermyCount += 1;
			}
			if (static_cast<CDefault_Enermy*>(*iter)->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_LEFT && static_cast<CDefault_Enermy*>(*iter)->Get_LeftLocation() == false)
			{
				static_cast<CDefault_Enermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
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
			if (static_cast<CBottomDirEnermy*>(*iter)->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_LEFT && static_cast<CBottomDirEnermy*>(*iter)->Get_LeftLocation() == true)
			{
				static_cast<CBottomDirEnermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_LEFT);
				static_cast<CBottomDirEnermy*>(*iter)->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_LEFT);
				m_iEnermyCount += 1;
			}
			if (static_cast<CBottomDirEnermy*>(*iter)->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_LEFT && static_cast<CBottomDirEnermy*>(*iter)->Get_LeftLocation() == false)
			{
				static_cast<CBottomDirEnermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
				m_iEnermyCount -= 1;
			}

		}
		break;
	}
	default:
		break;
	}
}

void CLeftLocation::Occupation(_int _ObjID)
{


	switch (_ObjID)
	{

	case OBJID::OBJID_DEFAULT_ALLY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (m_AllyOccupation >= 100.f)
				if (dynamic_cast<CDefault_Ally*>(*iter)->Get_LeftLocation() == true)
					dynamic_cast<CDefault_Ally*>(*iter)->Set_Action(AIACTION::AIACTION_WAIT);
			if (m_iEnermyCount >= 1 && m_iAllyCount >= 1)
				if (dynamic_cast<CDefault_Ally*>(*iter)->Get_LeftLocation() == true)
					dynamic_cast<CDefault_Ally*>(*iter)->Set_Action(AIACTION::AIACTION_ENERMY_IN_AREA);
		}
		break;
	}
	case OBJID::OBJID_DEFAULT_ENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (m_EnermyOccupation >= 100.f)
				if (static_cast<CDefault_Enermy*>(*iter)->Get_LeftLocation() == true)
					static_cast<CDefault_Enermy*>(*iter)->Set_Action(AIACTION::AIACTION_WAIT);
			if (m_iEnermyCount >= 1 && m_iAllyCount >= 1)
				if (static_cast<CDefault_Enermy*>(*iter)->Get_LeftLocation() == true)
					static_cast<CDefault_Enermy*>(*iter)->Set_Action(AIACTION::AIACTION_ENERMY_IN_AREA);
			if (m_iEnermyCount >= 1 && m_iAllyCount <= 0)
				if (static_cast<CDefault_Enermy*>(*iter)->Get_LeftLocation() == true)
					static_cast<CDefault_Enermy*>(*iter)->Set_Action(AIACTION::AIACTION_OCCOPATION);

		}
		break;
	}
	case OBJID::OBJID_BDENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (m_EnermyOccupation >= 100.f)
				if (static_cast<CBottomDirEnermy*>(*iter)->Get_LeftLocation() == true)
					static_cast<CBottomDirEnermy*>(*iter)->Set_Action(AIACTION::AIACTION_WAIT);
			if (m_iEnermyCount >= 1 && m_iAllyCount >= 1)
				if (static_cast<CBottomDirEnermy*>(*iter)->Get_LeftLocation() == true)
					static_cast<CBottomDirEnermy*>(*iter)->Set_Action(AIACTION::AIACTION_ENERMY_IN_AREA);
			if (m_iEnermyCount >= 1 && m_iAllyCount <= 0)
				if (static_cast<CBottomDirEnermy*>(*iter)->Get_LeftLocation() == true)
					static_cast<CBottomDirEnermy*>(*iter)->Set_Action(AIACTION::AIACTION_OCCOPATION);

		}
		break;
	}
	default:
		break;
	}
}

CLeftLocation* CLeftLocation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLeftLocation*		pInstance = new CLeftLocation(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("RightLocation Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CLeftLocation::Add_Component(void)
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

void CLeftLocation::Free(void)
{
	__super::Free();
}