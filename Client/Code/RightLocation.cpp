#include "stdafx.h"
#include "..\Header\RightLocation.h"

#include "Export_Function.h"
#include"Default_Enermy.h"
#include"Management.h"
#include"Layer.h"
#include"Default_Enermy.h"
#include"BottomDirEnermy.h"
#include"Default_Ally.h"
#include"TestPlayer.h"
#include"EnermyMgr.h"
#include"BottomDirAlly.h"
CRightLocation::CRightLocation(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CRightLocation::CRightLocation(const CRightLocation& rhs)
	: Engine::CGameObject(rhs)
{

}

CRightLocation::~CRightLocation()
{
}

HRESULT CRightLocation::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(VTXITV*(VTXCNTX - 1) *0.25, 0.f, VTXITV*(VTXCNTZ - 1) *0.25);
	m_pTransformCom->Set_Pos(VTXITV*(VTXCNTX - 1) *0.75, 0.f, VTXITV*(VTXCNTZ - 1) *0.25);

	m_LocationState = LOCATIONSTATE::STATE_NEUTRALITY;
	return S_OK;
}

_int CRightLocation::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	if (m_Test == false)
	{
		m_Test = true;
	}
	else
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);

		if (Temps.size() != 0)
		{
			for (auto iter = Temps.begin(); iter < Temps.end(); ++iter)
			{
				CheckObject(OBJID::OBJID_BDENERMY);
				Occupation(OBJID::OBJID_BDENERMY);
			}
		}
		Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
		{
			if (Temps.size() != 0)
			{
				for (auto iter = Temps.begin(); iter < Temps.end(); ++iter)
				{
					CheckObject(OBJID::OBJID_BDALLY);
					Occupation(OBJID::OBJID_BDALLY);
				}
			}
		}
		if (m_iEnermyCount >= 1 && m_iAllyCount <= 0)
			m_EnermyOccupation += 0.005f*m_iEnermyCount;
		if (m_iAllyCount >= 1 && m_iEnermyCount <= 0)
			m_AllyOccupation += 0.5f*m_iAllyCount;
		if (m_EnermyOccupation >= 100.f)
		{
			m_EnermyOccupation = 100.f;
			m_LocationState = LOCATIONSTATE::STATE_ENERMY;
		}
		if (m_AllyOccupation >= 100.f)
		{
			m_AllyOccupation = 100.f;
			m_LocationState = LOCATIONSTATE::STATE_ALLY;
		}
	}
	return OBJ_NOEVENT;
}

void CRightLocation::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_PRIORITY, this);
}

void CRightLocation::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pLocationCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}




void CRightLocation::CheckObject(_int _ObjID)
{

	switch (_ObjID)
	{
	case OBJID::OBJID_BDENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_RIGHT && dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightLocation() == true)
			{
				dynamic_cast<CBottomDirEnermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_RIGHT);
				dynamic_cast<CBottomDirEnermy*>(*iter)->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_RIGHT);
				m_iEnermyCount += 1;
			}
			if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_RIGHT && dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightLocation() == false)
			{
				dynamic_cast<CBottomDirEnermy*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
				m_iEnermyCount -= 1;
			}

		}
		break;
	}
	case OBJID::OBJID_BDALLY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (dynamic_cast<CBottomDirAlly*>(*iter)->Get_LocationCheck() != LOCATIONCHECK::LOCATIONCHECK_RIGHT && dynamic_cast<CBottomDirAlly*>(*iter)->Get_RightLocation() == true)
			{
				dynamic_cast<CBottomDirAlly*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_RIGHT);
				dynamic_cast<CBottomDirAlly*>(*iter)->Set_PastLocation(LOCATIONCHECK::LOCATIONCHECK_RIGHT);
				m_iAllyCount += 1;
			}
			if (dynamic_cast<CBottomDirAlly*>(*iter)->Get_LocationCheck() == LOCATIONCHECK::LOCATIONCHECK_RIGHT && dynamic_cast<CBottomDirAlly*>(*iter)->Get_RightLocation() == false)
			{
				dynamic_cast<CBottomDirAlly*>(*iter)->Set_LocationCheck(LOCATIONCHECK::LOCATIONCHECK_END);
				m_iAllyCount -= 1;
			}
		}
		break;
	}
	default:
		break;
	}
}
void CRightLocation::Occupation(_int _ObjID)
{
	switch (_ObjID)
	{
	case OBJID::OBJID_BDENERMY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightLocation() == true)
			{

				if (m_EnermyOccupation >= 100.f)
				{
					if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightLocation() == true)
						dynamic_cast<CBottomDirEnermy*>(*iter)->Set_Action(AIACTION::AIACTION_WAIT);
				}
				else if (m_iEnermyCount >= 1 && m_iAllyCount >= 1)
				{
					if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightLocation() == true)
						dynamic_cast<CBottomDirEnermy*>(*iter)->Set_Action(AIACTION::AIACTION_ENERMY_IN_AREA);
				}
				else if (m_iEnermyCount >= 1 && m_iAllyCount <= 0)
				{
					if (dynamic_cast<CBottomDirEnermy*>(*iter)->Get_RightLocation() == true)
						dynamic_cast<CBottomDirEnermy*>(*iter)->Set_Action(AIACTION::AIACTION_OCCOPATION);
				}
			}
		}
		break;
	}
	case OBJID::OBJID_BDALLY:
	{
		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			if (dynamic_cast<CBottomDirAlly*>(*iter)->Get_RightLocation() == true)
			{
				if (m_AllyOccupation >= 100.f)
				{
					if (dynamic_cast<CBottomDirAlly*>(*iter)->Get_RightLocation() == true)
						dynamic_cast<CBottomDirAlly*>(*iter)->Set_Action(AIACTION::AIACTION_WAIT);
				}
				else if (m_iAllyCount >= 1 && m_iEnermyCount <= 0)
				{
					if (dynamic_cast<CBottomDirAlly*>(*iter)->Get_RightLocation() == true)
						dynamic_cast<CBottomDirAlly*>(*iter)->Set_Action(AIACTION::AIACTION_OCCOPATION);
				}

				else if (m_iEnermyCount >= 1 && m_iAllyCount >= 1)
				{
					if (dynamic_cast<CBottomDirAlly*>(*iter)->Get_RightLocation() == true)
						dynamic_cast<CBottomDirAlly*>(*iter)->Set_Action(AIACTION::AIACTION_ENERMY_IN_AREA);
				}

			}
		}
		break;
	}
	}
}

CRightLocation* CRightLocation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRightLocation*		pInstance = new CRightLocation(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("RightLocation Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CRightLocation::Add_Component(void)
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
void CRightLocation::Free(void)
{
	__super::Free();
}