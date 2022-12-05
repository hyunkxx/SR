#include "stdafx.h"
#include "..\Header\PlayerManager.h"

#include "M3.h"
#include "Tank_01.h"
#include "Humvee.h"

IMPLEMENT_SINGLETON(CPlayerManager)

CPlayerManager::CPlayerManager()
	: m_pVehicle(nullptr)
{
}


CPlayerManager::~CPlayerManager()
{
	Free();
}

void CPlayerManager::Free(void)
{
}

void CPlayerManager::CreateVehicle(LPDIRECT3DDEVICE9 pGraphic, VEHICLE eType)
{
	//null일때만 Create사용
	assert(!m_pVehicle);

	switch (eType)
	{
	case Engine::VEHICLE::M3:
		m_pVehicle = CM3::Create(pGraphic);
		break;
	case Engine::VEHICLE::TANK1:
		m_pVehicle = CTank_01::Create(pGraphic);
		break;
	case Engine::VEHICLE::HUMVEE:
		m_pVehicle = CHumvee::Create(pGraphic);
		break;
	case Engine::VEHICLE::MAX:
		MSG_BOX("차량 인덱스 범위 초과");
		break;
	default:
		MSG_BOX("차량 인덱스 범위 초과");
		break;
	}
}
