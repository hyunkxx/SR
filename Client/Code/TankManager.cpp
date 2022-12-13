#include "stdafx.h"
#include "..\Header\TankManager.h"

#include "Humvee.h"
#include "SmallTank.h"
#include "MiddleTank.h"
#include "BigTank.h"
#include "LongTank.h"

IMPLEMENT_SINGLETON(CTankManager)

CTankManager::CTankManager()
	: m_pVehicle(nullptr)
{
	memset((void*)tankData, 0, sizeof(tankData));
	//	tankData[(UINT)VEHICLE::PANZER];

	InitalizeData();
}

CTankManager::~CTankManager()
{
	Free();
}

void CTankManager::Free(void)
{
}

void CTankManager::CreateVehicle(LPDIRECT3DDEVICE9 pGraphic, VEHICLE eType)
{
	if (m_pVehicle)
	{
		Safe_Release(m_pVehicle);
	}

	switch (eType)
	{
	case Engine::VEHICLE::HUMVEE:
		m_pVehicle = CHumvee::Create(pGraphic);
		break;
	case Engine::VEHICLE::SMALL_TANK:
		m_pVehicle = CSmallTank::Create(pGraphic);
		break;
	case Engine::VEHICLE::MIDDLE_TANK:
		m_pVehicle = CMiddleTank::Create(pGraphic);
		break;
	case Engine::VEHICLE::BIG_TANK:
		m_pVehicle = CBigTank::Create(pGraphic);
		break;
	case Engine::VEHICLE::LONG_TANK:
		m_pVehicle = CLongTank::Create(pGraphic);
		break;
	case Engine::VEHICLE::MAX:
		MSG_BOX("���� �ε��� ���� �ʰ�");
		break;
	default:
		MSG_BOX("���� �ε��� ���� �ʰ�");
		break;
	}
}

void CTankManager::InitalizeData()
{
	tankData[(UINT)VEHICLE::HUMVEE].strName = L"Humvee";
	tankData[(UINT)VEHICLE::HUMVEE].fDamage = 100.f;
	tankData[(UINT)VEHICLE::HUMVEE].fMaxHP = 300.f;
	tankData[(UINT)VEHICLE::HUMVEE].fCurHP = 300.f;
	tankData[(UINT)VEHICLE::HUMVEE].fSpeed = 0.f;;
	tankData[(UINT)VEHICLE::HUMVEE].fAccum = 0.f;
	tankData[(UINT)VEHICLE::HUMVEE].bAdvance = false;
	tankData[(UINT)VEHICLE::HUMVEE].bBack = false;
	//���ӵ�
	tankData[(UINT)VEHICLE::HUMVEE].fAccel_Ad = 6.f;
	tankData[(UINT)VEHICLE::HUMVEE].fAccel_Back = 6.f;
	//�ְ�ӵ� ����
	tankData[(UINT)VEHICLE::HUMVEE].fAccel_top_speed = 40.f;
	tankData[(UINT)VEHICLE::HUMVEE].fBack_top_speed = -15.f;
	//���� ����, ���� �ִ� ����, ȸ���ӵ� ����
	tankData[(UINT)VEHICLE::HUMVEE].fPosinDist = 1.f;
	tankData[(UINT)VEHICLE::HUMVEE].fLowAngle = D3DXToRadian(0.f);
	tankData[(UINT)VEHICLE::HUMVEE].TopAngle = D3DXToRadian(-40.f);
	tankData[(UINT)VEHICLE::HUMVEE].RotSpeed = 40.f;
	// ���� �ӵ�
	tankData[(UINT)VEHICLE::HUMVEE].fReload = 0.1f;
	tankData[(UINT)VEHICLE::HUMVEE].fReloadTime = 0.1f;
	tankData[(UINT)VEHICLE::HUMVEE].iCannonSpeed = 400;


	tankData[(UINT)VEHICLE::SMALL_TANK].strName = L"CV90 ������";
	tankData[(UINT)VEHICLE::SMALL_TANK].fDamage = 100.f;
	tankData[(UINT)VEHICLE::SMALL_TANK].fMaxHP = 500.f;
	tankData[(UINT)VEHICLE::SMALL_TANK].fCurHP = 500.f;
	tankData[(UINT)VEHICLE::SMALL_TANK].fSpeed = 0.f;;
	tankData[(UINT)VEHICLE::SMALL_TANK].fAccum = 0.f;
	tankData[(UINT)VEHICLE::SMALL_TANK].bAdvance = false;
	tankData[(UINT)VEHICLE::SMALL_TANK].bBack = false;
	//���ӵ�
	tankData[(UINT)VEHICLE::SMALL_TANK].fAccel_Ad = 6.f;
	tankData[(UINT)VEHICLE::SMALL_TANK].fAccel_Back = 4.f;
	//�ְ�ӵ� ����
	tankData[(UINT)VEHICLE::SMALL_TANK].fAccel_top_speed = 30.f;
	tankData[(UINT)VEHICLE::SMALL_TANK].fBack_top_speed = -10.f;
	//���� ����, ���� �ִ� ����, ȸ���ӵ� ����
	tankData[(UINT)VEHICLE::SMALL_TANK].fPosinDist = 1.5f;
	tankData[(UINT)VEHICLE::SMALL_TANK].fLowAngle = D3DXToRadian(0.f);
	tankData[(UINT)VEHICLE::SMALL_TANK].TopAngle = D3DXToRadian(-40.f);
	tankData[(UINT)VEHICLE::SMALL_TANK].RotSpeed = 40.f;
	// ���� �ӵ�
	tankData[(UINT)VEHICLE::SMALL_TANK].fReload = 0.5f;
	tankData[(UINT)VEHICLE::SMALL_TANK].fReloadTime = 0.5f;
	tankData[(UINT)VEHICLE::SMALL_TANK].iCannonSpeed = 500;

	tankData[(UINT)VEHICLE::MIDDLE_TANK].strName = L"T62 ��������";
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fDamage = 100.f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fMaxHP = 1000.f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fCurHP = 1000.f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fSpeed = 0.f;;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fAccum = 0.f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].bAdvance = false;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].bBack = false;
	//���ӵ�
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fAccel_Ad = 5.f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fAccel_Back = 3.f;
	//�ְ�ӵ� ����
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fAccel_top_speed = 20.f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fBack_top_speed = -10.f;
	//���� ����, ���� �ִ� ����, ȸ���ӵ� ����
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fPosinDist = 1.5f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fLowAngle = D3DXToRadian(0.f);
	tankData[(UINT)VEHICLE::MIDDLE_TANK].TopAngle = D3DXToRadian(-40.f);
	tankData[(UINT)VEHICLE::MIDDLE_TANK].RotSpeed = 30.f;
	// ���� �ӵ�
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fReload = 2.5f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].fReloadTime = 2.5f;
	tankData[(UINT)VEHICLE::MIDDLE_TANK].iCannonSpeed = 700;

	tankData[(UINT)VEHICLE::BIG_TANK].strName = L"Tiger ������";
	tankData[(UINT)VEHICLE::BIG_TANK].fDamage = 100.f;
	tankData[(UINT)VEHICLE::BIG_TANK].fMaxHP = 2000.f;
	tankData[(UINT)VEHICLE::BIG_TANK].fCurHP = 2000.f;
	tankData[(UINT)VEHICLE::BIG_TANK].fSpeed = 0.f;;
	tankData[(UINT)VEHICLE::BIG_TANK].fAccum = 0.f;
	tankData[(UINT)VEHICLE::BIG_TANK].bAdvance = false;
	tankData[(UINT)VEHICLE::BIG_TANK].bBack = false;
	//���ӵ�
	tankData[(UINT)VEHICLE::BIG_TANK].fAccel_Ad = 3.f;
	tankData[(UINT)VEHICLE::BIG_TANK].fAccel_Back = 2.f;
	//�ְ�ӵ� ����
	tankData[(UINT)VEHICLE::BIG_TANK].fAccel_top_speed = 15.f;
	tankData[(UINT)VEHICLE::BIG_TANK].fBack_top_speed = -7.5f;
	//���� ����, ���� �ִ� ����, ȸ���ӵ� ����
	tankData[(UINT)VEHICLE::BIG_TANK].fPosinDist = 2.f;
	tankData[(UINT)VEHICLE::BIG_TANK].fLowAngle = D3DXToRadian(0.f);
	tankData[(UINT)VEHICLE::BIG_TANK].TopAngle = D3DXToRadian(-40.f);
	tankData[(UINT)VEHICLE::BIG_TANK].RotSpeed = 20.f;
	// ���� �ӵ�
	tankData[(UINT)VEHICLE::BIG_TANK].fReload = 5.f;
	tankData[(UINT)VEHICLE::BIG_TANK].fReloadTime = 5.f;
	tankData[(UINT)VEHICLE::BIG_TANK].iCannonSpeed = 800;

	tankData[(UINT)VEHICLE::LONG_TANK].strName = L"K-9 ���ְ����";
	tankData[(UINT)VEHICLE::LONG_TANK].fDamage = 100.f;
	tankData[(UINT)VEHICLE::LONG_TANK].fMaxHP = 1200.f;
	tankData[(UINT)VEHICLE::LONG_TANK].fCurHP = 1100.f;
	tankData[(UINT)VEHICLE::LONG_TANK].fSpeed = 0.f;;
	tankData[(UINT)VEHICLE::LONG_TANK].fAccum = 0.f;
	tankData[(UINT)VEHICLE::LONG_TANK].bAdvance = false;
	tankData[(UINT)VEHICLE::LONG_TANK].bBack = false;
	//���ӵ�
	tankData[(UINT)VEHICLE::LONG_TANK].fAccel_Ad = 2.f;
	tankData[(UINT)VEHICLE::LONG_TANK].fAccel_Back = 2.f;
	//�ְ�ӵ� ����
	tankData[(UINT)VEHICLE::LONG_TANK].fAccel_top_speed = 10.f;
	tankData[(UINT)VEHICLE::LONG_TANK].fBack_top_speed = -10.f;
	//���� ����, ���� �ִ� ����, ȸ���ӵ� ����
	tankData[(UINT)VEHICLE::LONG_TANK].fPosinDist = 2.f;
	tankData[(UINT)VEHICLE::LONG_TANK].fLowAngle = D3DXToRadian(0.f);
	tankData[(UINT)VEHICLE::LONG_TANK].TopAngle = D3DXToRadian(-40.f);
	tankData[(UINT)VEHICLE::LONG_TANK].RotSpeed = 10.f;
	// ���� �ӵ�
	tankData[(UINT)VEHICLE::LONG_TANK].fReload = 6.f;
	tankData[(UINT)VEHICLE::LONG_TANK].fReloadTime = 6.f;
	tankData[(UINT)VEHICLE::LONG_TANK].iCannonSpeed = 1100;
}
