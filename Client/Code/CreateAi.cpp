#include "stdafx.h"
#include "..\Header\CreateAi.h"
#include"Export_Function.h"
#include"Default_Ally.h"
#include"Default_Enermy.h"
#include"BottomDirAlly.h"
#include"BottomDirEnermy.h"
#include"EnermyMgr.h"
CCreateAi::CCreateAi(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	ZeroMemory(&m_eData, sizeof(m_eData));
}

CCreateAi::CCreateAi(const CCreateAi & rhs)
	: CGameObject(rhs)
{
}

CCreateAi::~CCreateAi()
{
}


HRESULT CCreateAi::Ready_Object(void)
{
	Enermy = CGameMode::GetInstance();
	return S_OK;
}

_int CCreateAi::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);
	m_CoolTime += fTimeDelta;
	CheckFieldCount();
	if (Field <= 12)
		Produce(fTimeDelta);

	return 0;
}
void CCreateAi::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CCreateAi::Render_Object(void)
{

}

void CCreateAi::CheckFieldCount(void)
{
	vector<CGameObject*> Default_EnermyGather = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID_DEFAULT_ENERMY);
	vector<CGameObject*> BottomDirEnermyGather = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID_BDENERMY);
	if (Default_EnermyGather.size() != 0)
	{
		for (auto& iter = Default_EnermyGather.begin(); iter < Default_EnermyGather.end(); iter++)
		{
			if ((*iter) != nullptr)
			{
				if (!(dynamic_cast<CDefault_Enermy*>(*iter)->Get_CreateCheck()))
				{
					dynamic_cast<CDefault_Enermy*>(*iter)->Set_CreateCheck(true);
					Field += 1;
				}
			}
		}
	}
	if (BottomDirEnermyGather.size() != 0)
	{
		for (auto& iter = BottomDirEnermyGather.begin(); iter < BottomDirEnermyGather.end(); iter++)
		{
			if ((*iter) != nullptr)
			{
				if (!(dynamic_cast<CBottomDirEnermy*>(*iter)->Get_CreateCheck()))
				{
					dynamic_cast<CBottomDirEnermy*>(*iter)->Set_CreateCheck(true);
					Field += 1;
				}
			}
		}
	}
}

void CCreateAi::Produce(_float fTimeDelta)
{
	if (m_CoolTime >= 3.f)
	{
		Random = 0;
		if (Field >= 7)
		{
			if (!Check)
			{
				Random = rand() % 2 + 1;//1은 뽑기,2는 대기
				Check = true; //체크는 상대가 카운터가 낮아지거나 내가 낮아질때 다시한번더 기회
			}
		}
		else//field에 너무 없으면 그냥뽑아
		{
			Random = 1;
		}
		if (Random == 1)
		{
			if (Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] >= 2500)
			{
				_int Temp = rand() % 3;
				switch (Temp)
				{
				case 0:
					m_eData.TankType = TANKTYPE::BIG_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 2200;
					break;
				case 1:
					m_eData.TankType = TANKTYPE::BIG_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 2200;
					break;
				case 2:
					m_eData.TankType = TANKTYPE::MIDDLE_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 2000;
					break;
				}
				m_eData.vPos.x = (float)(VTXITV*VTXCNTX - rand() % 110 - 50.f);
				m_eData.vPos.y = 0;
				m_eData.vPos.z = (float)(VTXITV*VTXCNTZ - rand() % 50 - 50.f);
				_int Sour = rand() % 2;
				if (Sour == 0)
				{
					m_eData.eID = OBJID::OBJID_BDENERMY;
					CGameObject* pEnermy = CBottomDirEnermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDENERMY);
				}
				else
				{
					m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
					CGameObject* pEnermy = CDefault_Enermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
				}
			}
			else if (Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] >= 2200)
			{
				_int Temp = rand() % 3;
				switch (Temp)
				{
				case 0:
					m_eData.TankType = TANKTYPE::BIG_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 2200;
					break;
				case 1:
					m_eData.TankType = TANKTYPE::SMALL_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 800;
					break;
				case 2:
					m_eData.TankType = TANKTYPE::MIDDLE_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 2000;
					break;
				}
				m_eData.vPos.x = (float)(VTXITV*VTXCNTX - rand() % 110 - 50.f);
				m_eData.vPos.y = 0;
				m_eData.vPos.z = (float)(VTXITV*VTXCNTZ - rand() % 50 - 50.f);
				_int Sour = rand() % 2;
				if (Sour == 0)
				{
					m_eData.eID = OBJID::OBJID_BDENERMY;
					CGameObject* pEnermy = CBottomDirEnermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDENERMY);
				}
				else
				{
					m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
					CGameObject* pEnermy = CDefault_Enermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
				}
			}
			else if (Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] >= 2000)
			{
				_int Temp = rand() % 3;
				switch (Temp)
				{
				case 0:
					m_eData.TankType = TANKTYPE::HUMVEE;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 200;
					break;
				case 1:
					m_eData.TankType = TANKTYPE::SMALL_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 800;
					break;
				case 2:
					m_eData.TankType = TANKTYPE::MIDDLE_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 2000;
					break;
				}
				m_eData.vPos.x = (float)(VTXITV*VTXCNTX - rand() % 110 - 50.f);
				m_eData.vPos.y = 0;
				m_eData.vPos.z = (float)(VTXITV*VTXCNTZ - rand() % 50 - 50.f);
				_int Sour = rand() % 2;
				if (Sour == 0)
				{
					m_eData.eID = OBJID::OBJID_BDENERMY;
					CGameObject* pEnermy = CBottomDirEnermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDENERMY);
				}
				else
				{
					m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
					CGameObject* pEnermy = CDefault_Enermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
				}
			}
			else if (Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] >= 800)
			{
				_int Temp = rand() % 2;
				switch (Temp)
				{
				case 0:
					m_eData.TankType = TANKTYPE::HUMVEE;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 200;
					break;
				case 1:
					m_eData.TankType = TANKTYPE::SMALL_TANK;
					Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 800;
					break;
				}
				m_eData.vPos.x = (float)(VTXITV*VTXCNTX - rand() % 110 - 50.f);
				m_eData.vPos.y = 0;
				m_eData.vPos.z = (float)(VTXITV*VTXCNTZ - rand() % 50 - 50.f);
				_int Sour = rand() % 2;
				if (Sour == 0)
				{
					m_eData.eID = OBJID::OBJID_BDENERMY;
					CGameObject* pEnermy = CBottomDirEnermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDENERMY);
				}
				else
				{
					m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
					CGameObject* pEnermy = CDefault_Enermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
				}
			}
			else if (Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] >= 200)
			{
				m_eData.TankType = TANKTYPE::HUMVEE;
				m_eData.vPos.x = (float)(VTXITV*VTXCNTX - rand() % 110 - 50.f);
				m_eData.vPos.y = 0;
				m_eData.vPos.z = (float)(VTXITV*VTXCNTZ - rand() % 50 - 50.f);
				_int Temp = rand() % 2;
				if (Temp == 0)
				{
					m_eData.eID = OBJID::OBJID_BDENERMY;
					CGameObject* pEnermy = CBottomDirEnermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDENERMY);
				}
				else
				{
					m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
					CGameObject* pEnermy = CDefault_Enermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
				}
				Enermy->m_nGold[(UINT)(CGameMode::TYPE::ENEMY)] -= 200;
			}


			m_CoolTime = 0.f;
		}
	}
}

CCreateAi * CCreateAi::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCreateAi*		pInstance = new CCreateAi(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CCreateAi Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCreateAi::Free(void)
{
	__super::Free();
}

